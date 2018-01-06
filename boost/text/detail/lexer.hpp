#ifndef BOOST_TEXT_DETAIL_LEXER_HPP
#define BOOST_TEXT_DETAIL_LEXER_HPP

#include <boost/text/string.hpp>
#include <boost/text/utf8.hpp>
#include <boost/text/utility.hpp>

#include <sstream>
#include <iomanip>


namespace boost { namespace text {

    // TODO: These should go in parser_fwd.hpp.

    using parser_diagnostic_callback = std::function<void(string const &)>;

    struct parse_error : std::exception
    {
        parse_error(string_view msg, int line, int column) :
            msg_(msg),
            line_(line),
            column_(column)
        {}
        char const * what() const noexcept { return msg_.begin(); }
        int line() const { return line_; }
        int column() const { return column_; }

    private:
        string msg_;
        int line_;
        int column_;
    };

}}

namespace boost { namespace text { namespace detail {

    enum class token_kind {
        code_point,

        // Code point ranges like "x-y" appear after abbreviated relations,
        // but "-" is fine as a regular code point elsewhere.  The lexer does
        // not have the necessary context to distinguish these two cases.  To
        // resolve this, a "-" inside quotes or escaped ("\-") is treated as a
        // regular code point, but is otherwise a special dash token.
        dash,

        and_,
        or_,
        slash,
        open_bracket,
        close_bracket,
        equal,
        primary_before,
        secondary_before,
        tertiary_before,
        quaternary_before,
        equal_star,
        primary_before_star,
        secondary_before_star,
        tertiary_before_star,
        quaternary_before_star,
        identifier
    };

#ifndef NDEBUG
    inline std::ostream & operator<<(std::ostream & os, token_kind kind)
    {
#    define CASE(x)                                                            \
    case token_kind::x: os << #x; break
        switch (kind) {
            CASE(code_point);
            CASE(dash);
            CASE(and_);
            CASE(or_);
            CASE(slash);
            CASE(equal);
            CASE(open_bracket);
            CASE(close_bracket);
            CASE(primary_before);
            CASE(secondary_before);
            CASE(tertiary_before);
            CASE(quaternary_before);
            CASE(primary_before_star);
            CASE(secondary_before_star);
            CASE(tertiary_before_star);
            CASE(quaternary_before_star);
            CASE(equal_star);
            CASE(identifier);
        }
#    undef CASE
        return os;
    }
#endif

    struct token
    {
        token() : kind_(token_kind::identifier), cp_(), line_(-1), column_(-1)
        {}
        token(token_kind kind, int line, int column) :
            kind_(kind),
            cp_(),
            line_(line),
            column_(column)
        {}
        token(uint32_t c, int line, int column) :
            kind_(token_kind::code_point),
            cp_(c),
            line_(line),
            column_(column)
        {}
        token(string identifier, int line, int column) :
            kind_(token_kind::identifier),
            cp_(),
            identifier_(std::move(identifier)),
            line_(line),
            column_(column)
        {}

        token_kind kind() const { return kind_; }
        uint32_t cp() const
        {
            assert(kind_ == token_kind::code_point);
            return cp_;
        }
        string const & identifier() const { return identifier_; }
        int line() const { return line_; }
        int column() const { return column_; }

        friend bool operator==(token const & tok, uint32_t cp)
        {
            return tok.kind() == token_kind::code_point && tok.cp() == cp;
        }

        friend bool operator==(token const & tok, token_kind kind)
        {
            assert(
                kind != token_kind::code_point &&
                kind != token_kind::identifier);
            return tok.kind() == kind;
        }

        friend bool operator==(token const & tok, string_view id)
        {
            return tok.kind() == token_kind::identifier &&
                   tok.identifier() == id;
        }

#ifndef NDEBUG
        friend std::ostream & operator<<(std::ostream & os, token tok)
        {
            os << "[" << tok.kind_ << " ";
            if (tok.kind_ == token_kind::code_point)
                os << std::hex << "cp=0x" << tok.cp_ << std::dec << " ";
            else if (tok.kind_ == token_kind::identifier)
                os << "\"" << tok.identifier_ << "\" ";
            os << "(" << tok.line_ << ":" << tok.column_ << ")]";
            return os;
        }
#endif

    private:
        token_kind kind_;
        uint32_t cp_;
        string identifier_;
        int line_;
        int column_;
    };

    using tokens_t = std::vector<token>;
    using token_iter = tokens_t::const_iterator;

    struct lines_and_tokens
    {
        std::vector<int> line_starts_;
        tokens_t tokens_;
    };

#ifndef NDEBUG
    std::ostream &
    dump(std::ostream & os, lines_and_tokens const & lat, string_view str)
    {
        auto tok_it = lat.tokens_.begin();
        for (int i = 0, end = lat.line_starts_.size(); i < end; ++i) {
            bool const last_line = end <= i + 1;
            auto tok_end = lat.tokens_.end();
            if (last_line) {
                os << (str.begin() + lat.line_starts_[i]);
                if (str[-1] != '\n')
                    os << "\n";
            } else {
                os.write(
                    str.begin() + lat.line_starts_[i],
                    lat.line_starts_[i + 1] - lat.line_starts_[i]);
                if (str[lat.line_starts_[i + 1] - 1] != '\n')
                    os << "\n";
                tok_end = std::find_if(tok_it, tok_end, [i](token const & tok) {
                    return i < tok.line();
                });
            }
            auto pos = 0;
            for (; tok_it != tok_end; ++tok_it) {
                for (auto new_pos = tok_it->column(); pos < new_pos; ++pos) {
                    os << ' ';
                }
                os << '^';
                ++pos;
            }
            os << "\n";
        }
        return os;
    }
#endif

    enum class diag_kind { error, warning, note };

    inline string parse_diagnostic(
        diag_kind kind,
        string_view msg,
        int line,
        int column,
        string_view sv,
        std::vector<int> const & line_starts,
        string_view filename = string_view())
    {
        auto to_string = [](int i) {
            std::ostringstream os;
            os << i;
            return string(os.str());
        };

        string str;
        if (!filename.empty()) {
            str += filename;
            str += ":";
        }
        str += to_string(line + 1);
        str += ":";
        str += to_string(column);
        str += ":";
        if (kind == diag_kind::error)
            str += "error: ";
        else if (kind == diag_kind::warning)
            str += "warning: ";
        else
            str += "note: ";
        str += msg;
        if (str.empty() || str[-1] != '\n')
            str += '\n';

        auto const line_start = sv.begin() + line_starts[line];
        auto const line_end = std::find(line_start, sv.end(), '\n');
        str.insert(str.end(), line_start, line_end);
        str += '\n';
        str += repeated_string_view(" ", column);
        str += "^\n";

        return str;
    }

    inline lines_and_tokens
    lex(char const * first,
        char const * const last,
        parser_diagnostic_callback errors,
        string_view filename = "")
    {
        lines_and_tokens retval;

        auto const initial_first = first;

        retval.line_starts_.push_back(0);

        int char_index = 0;
        int line = 0;
        int column = 0;
        int initial_column = column;
        int brackets_nesting = 0;
        bool in_quote = false;

        char buf[1024];

        auto report_error = [&](string_view msg, int column) {
            if (errors) {
                errors(parse_diagnostic(
                    diag_kind::error,
                    msg,
                    line,
                    column,
                    string_view(initial_first, last - initial_first),
                    retval.line_starts_,
                    filename));
            }
            throw parse_error(msg, line, column);
        };

        auto check_not_at_end = [&](string_view msg) {
            if (first == last)
                report_error(msg, column);
        };

        auto check_not_in_quote = [&](string_view sv) {
            string msg("Unquoted newline character '");
            msg += sv;
            msg += "' may not appear inside of a quote";
            if (in_quote)
                report_error(msg, column);
        };

        auto consume =
            [&](int n, string_view end_msg, char * buf_ptr = nullptr) {
                char * buf_ = buf_ptr ? buf_ptr : buf;
                while (n) {
                    check_not_at_end(end_msg);
                    *buf_++ = *first++;
                    ++column;
                    ++char_index;
                    --n;
                }
                return buf_;
            };

        auto consume_one = [&](string_view end_msg) {
            return *(consume(1, end_msg) - 1);
        };

        auto consume_if_equals = [&](char c) {
            bool retval = false;
            if (first != last && *first == c) {
                consume_one("");
                retval = true;
            }
            return retval;
        };

        auto is_space = [&](char initial_char) {
            auto const code_units = utf8::code_point_bytes(initial_char);
            if (code_units < 0 || last - first < code_units - 1)
                return 0;
            utf32_range as_utf32(first - 1, first - 1 + code_units);
            uint32_t const cp = *as_utf32.begin();

            // Unicode Pattern_White_Space
            // See
            // http://unicode.org/cldr/utility/list-unicodeset.jsp?a=%5B:Pattern_White_Space:%5D&abb=on&g=
            // Intentionally leaves out \r (0xd) and \n (0xa) for line tracking
            // purposes.
            auto retval = 0;
            if ((0x9 <= cp && cp <= 0xc && cp != 0xa) || cp == 0x20 ||
                cp == 0x85 || cp == 0x200e || cp == 0x200f || cp == 0x2028 ||
                cp == 0x2029) {
                retval = cp;
            }
            if (retval)
                consume(code_units - 1, "");
            return retval;
        };

        auto is_syntax_char = [](char c) {
            // \u0021-\u002F \u003A-\u0040 \u005B-\u0060 \u007B-\u007E
            // Intentionally leaves out 0x21 (' '), 0x27 ('\''), 0x2d ('-'),
            // and 0x5c ('\\'), which are all handled specially.
            return (0x22 <= c && c <= 0x2f && c != 0x27 && c != 0x2d) ||
                   (0x3a <= c && c <= 0x40) ||
                   (0x5b <= c && c <= 0x60 && c != 0x5c) ||
                   (0x7b <= c && c <= 0x7e);
        };

        auto is_id_char = [](char c) {
            char const id_chars[] =
                "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUV"
                "WXYZ"
                "_-";
            return std::find(std::begin(id_chars), std::end(id_chars), c) !=
                   std::end(id_chars);
        };

        auto is_hex = [](char c) {
            return ('0' <= c && c <= '9') || ('a' <= c && c <= 'f') ||
                   ('A' <= c && c <= 'F');
        };

        auto is_octal = [](char c) { return '0' <= c && c <= '8'; };

        auto from_hex = [](char const * str) {
            std::istringstream is(str);
            uint32_t x;
            is >> std::hex >> x;
            return x;
        };

        auto from_octal = [](char const * str) {
            std::istringstream is(str);
            uint32_t x;
            is >> std::oct >> x;
            return x;
        };

        auto newline = [&]() {
            ++line;
            column = 0;
            retval.line_starts_.push_back(char_index);
        };

        auto push = [&](token_kind kind) {
            retval.tokens_.push_back(token(kind, line, initial_column));
        };

        auto push_cp = [&](uint32_t cp) {
            retval.tokens_.push_back(token(cp, line, initial_column));
        };

        auto push_identifier = [&](string id) {
            retval.tokens_.push_back(
                token(std::move(id), line, initial_column));
        };

        auto lex_utf8 = [&](char initial_char) {
            // UTF-8 encoded code point.
            auto const code_units = utf8::code_point_bytes(initial_char);
            if (code_units < 0)
                report_error("Invalid initial UTF-8 code unit", initial_column);
            *buf = initial_char;
            if (1 < code_units) {
                consume(code_units - 1, "Incomplete UTF-8 sequence", buf + 1);
            }
            utf32_range as_utf32(buf, buf + code_units);
            auto const cp = *as_utf32.begin();
            if (!in_quote && cp == '-')
                push(token_kind::dash);
            else
                push_cp(cp);
            // Treat this cp as a single column for error reporting purposes,
            // even though this looks weird for wide glyphs, such as in East
            // Asian scripts.
            column = initial_column + 1;
        };

        while (first != last) {
            initial_column = column;
            char const initial_char = consume_one("");

            if (is_syntax_char(initial_char)) {
                if (in_quote) {
                    push_cp(initial_char);
                } else if (initial_char == '&') {
                    push(token_kind::and_);
                } else if (initial_char == '|') {
                    push(token_kind::or_);
                } else if (initial_char == '/') {
                    push(token_kind::slash);
                } else if (initial_char == '=') {
                    if (consume_if_equals('*'))
                        push(token_kind::equal_star);
                    else
                        push(token_kind::equal); // TODO
                } else if (initial_char == '[') {
                    push(token_kind::open_bracket);
                    ++brackets_nesting;
                } else if (initial_char == ']') {
                    push(token_kind::close_bracket);
                    if (--brackets_nesting < 0) {
                        report_error(
                            "Close bracket without matching open bracket",
                            initial_column);
                    }
                } else if (initial_char == '<') {
                    token_kind kind = token_kind::primary_before;
                    if (consume_if_equals('<')) {
                        kind = token_kind::secondary_before;
                        if (consume_if_equals('<')) {
                            kind = token_kind::tertiary_before;
                            if (consume_if_equals('<')) {
                                kind = token_kind::quaternary_before;
                            }
                        }
                    }
                    if (consume_if_equals('*'))
                        kind =
                            static_cast<token_kind>(static_cast<int>(kind) + 5);
                    push(kind);
                } else if (initial_char == '#') {
                    while (first != last && *first != '\n') {
                        consume_one("");
                    }
                    if (first != last)
                        consume_one("");
                    newline();
                } else {
                    string msg("Unescaped syntax character \'");
                    msg += initial_char;
                    msg += "\'";
                    report_error(msg, initial_column);
                }
            } else if (!in_quote && is_space(initial_char)) {
                continue;
            } else if (initial_char == '\r') {
                check_not_in_quote("\\r");
                char const c = consume_one(
                    "'\\r' at end of input (must be followed by '\\n')");
                if (c != '\n') {
                    report_error(
                        "Stray '\\r' without following '\\n'", initial_column);
                }
                newline();
            } else if (initial_char == '\n') {
                check_not_in_quote("\\n");
                newline();
            } else if (initial_char == '\\') {
                // The escape-handling logic here is derived from the
                // documentation of ICU's UnicodeString::unescape(), as
                // indicated by
                // http://www.unicode.org/reports/tr35/tr35-collation.html#Rules
                char const c = consume_one("\\ at end of input");
                assert(c != 'r' && c != 'n'); // Handled above.
                switch (c) {
                case 'u': {
                    auto buf_end = consume(
                        4, "Incomplete '\\uNNNN' hexidecimal escape sequence");
                    if (!std::all_of(buf, buf_end, is_hex)) {
                        report_error(
                            "Non-hexidecimal digit in '\\uNNNN' hexidecimal "
                            "escape sequence",
                            initial_column);
                    }
                    *buf_end = '\0';
                    push_cp(from_hex(buf));
                    break;
                }
                case 'U': {
                    auto buf_end = consume(
                        8,
                        "Incomplete '\\UNNNNNNNN' hexidecimal escape "
                        "sequence");
                    if (!std::all_of(buf, buf_end, is_hex)) {
                        report_error(
                            "Non-hexidecimal digit in '\\UNNNNNNNN' hexidecimal "
                            "escape sequence",
                            initial_column);
                    }
                    *buf_end = '\0';
                    push_cp(from_hex(buf));
                    break;
                }
                case 'x': {
                    char local_buf[4] = {0};
                    auto local_buf_end = local_buf;
                    *local_buf_end++ = consume_one(
                        "Incomplete '\\xN[N]' hexidecimal escape sequence "
                        "(at least one hexidecimal digit must follow '\\x')");
                    if (!is_hex(local_buf[0])) {
                        report_error(
                            "Non-octal hexidecimal in '\\xN[N]' hexidecimal "
                            "escape sequence",
                            initial_column);
                    }
                    if (first != last && is_hex(*first))
                        *local_buf_end++ = consume_one("");
                    *local_buf_end = '\0';
                    push_cp(from_hex(local_buf));
                    break;
                }
                case 'o': {
                    char local_buf[4] = {0};
                    auto local_buf_end = local_buf;
                    *local_buf_end++ = consume_one(
                        "Incomplete '\\oN[N][N]' octal escape sequence (at "
                        "least one octal digit must follow '\\o')");
                    if (!is_octal(local_buf[0])) {
                        report_error(
                            "Non-octal digit in '\\oN[N][N]' octal escape "
                            "sequence",
                            initial_column);
                    }
                    if (first != last && is_octal(*first))
                        *local_buf_end++ = consume_one("");
                    if (first != last && is_octal(*first))
                        *local_buf_end++ = consume_one("");
                    *local_buf_end = '\0';
                    push_cp(from_octal(local_buf));
                    break;
                }
                case 'a': push_cp(0x7); break;
                case 'b': push_cp(0x8); break;
                case 't': push_cp(0x9); break;
                case 'v': push_cp(0xb); break;
                case 'f': push_cp(0xc); break;
                case 'e': push_cp(0x1b); break;
                case '"': push_cp(0x22); break;
                case '\'': push_cp(0x27); break;
                case '?': push_cp(0x3f); break;
                case '\\': push_cp(0x5c); break;
                case '-': push_cp(c); break;
                default: lex_utf8(c); break;
                }
            } else if (initial_char == '\'') {
                if (first != last && *first == '\'') {
                    consume_one("");
                    push_cp('\'');
                } else {
                    in_quote = !in_quote;
                }
            } else if (
                !in_quote && brackets_nesting == 1 &&
                is_id_char(initial_char)) {
                string str;
                str += initial_char;
                while (first != last && is_id_char(*first)) {
                    str += consume_one("");
                }
                push_identifier(std::move(str));
            } else {
                lex_utf8(initial_char);
            }
        }

        if (in_quote)
            report_error("Close quote missing at end of input", column);

        if (0 < brackets_nesting)
            report_error("Close bracket(s) missing at end of input", column);

        if (!retval.tokens_.empty() && retval.tokens_.back() == 0)
            retval.tokens_.pop_back();

        return retval;
    }

}}}

#endif
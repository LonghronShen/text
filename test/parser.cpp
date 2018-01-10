#include <boost/text/detail/parser.hpp>

#include "parser_tests.hpp"

#include <gtest/gtest.h>


using namespace boost;

TEST(parser, exceptions)
{
    text::detail::collation_tailoring_interface callbacks = {
        [](text::detail::cp_seq_t const & reset_, bool before_) {},
        [](text::detail::relation_t const & rel) {},
        [](text::collation_strength strength) {},
        [](text::variable_weighting weighting) {},
        [](text::l2_weight_order order) {},
        [](text::detail::cp_seq_t const & suppressions) {},
        [](std::vector<text::string> && reorderings, bool simple) {},
        [](text::string const & s) { std::cout << s; },
        [](text::string const & s) { std::cout << s; }};

    // Exceptions produced by parse() itself.
    {
        text::string_view sv = "";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "a";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    // Exceptions produced by parse_rule()
    {
        text::string_view sv = "& \\ufffd < a";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a < \\ufffd";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a <* \\ufffd";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a <* \\ufffc-\\ufffd";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a <* \\ufffc-\\U00010000";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a <* -";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }
    {
        text::string_view sv = "& a <* a-";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }
    {
        text::string_view sv = "& a <* a--";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "&[before a] a < b";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "&[before 1";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& [last implicit] < b";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& [first trailing] < b";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& [last trailing] < b";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& [first foo] < b";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& [first tertiary ignorable";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a < b |";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a <";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a <*";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a &";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a <* \\u0300";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& [before 1] a <<* b";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& [before 3] a < b";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "&";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "& a";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    // Exceptions produced by parse_option().
    {
        text::string_view sv = "[]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[foo]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[|]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[import foo]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[import";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[optimize [foo";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[optimize [foo]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[optimize [";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[optimize";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[optimize []";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[suppressContractions [foo";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[suppressContractions [foo]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[suppressContractions";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[suppressContractions [";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[suppressContractions []";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[strength]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[strength";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[strength foo]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[strength I";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[alternate]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[alternate";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[alternate foo]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[alternate shifted";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[backwards]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[backwards";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[backwards 1]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[backwards 2";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder space] [reorder punct]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder Common]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder Inherited]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder Zzzz others]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder Grek Grek]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder rando_calrissian]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }

    {
        text::string_view sv = "[reorder Sinh Mtei Sylo Saur Kthi]";
        EXPECT_THROW(
            text::detail::parse(
                sv.begin(), sv.end(), callbacks, "<test-string>"),
            text::parse_error);
    }
}

TEST(parser, options)
{
    {
        text::detail::cp_seq_t result;
        text::detail::cp_seq_t const expected = {'a', 'b', 'c'};
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [&result](text::detail::cp_seq_t const & suppressions) {
                result = suppressions;
            },
            [](std::vector<text::string> && reorderings, bool simple) {},
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};

        text::string_view sv = "[suppressContractions [abc]]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, expected);
    }

    {
        text::collation_strength result = text::collation_strength::identical;
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [&result](text::collation_strength strength) { result = strength; },
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [](std::vector<text::string> && reorderings, bool simple) {},
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};

        text::string_view sv;

        sv = "[strength 1]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, text::collation_strength::primary);

        sv = "[strength 2]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, text::collation_strength::secondary);

        sv = "[strength 3]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, text::collation_strength::tertiary);

        sv = "[strength 4]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, text::collation_strength::quaternary);

        sv = "[strength I]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, text::collation_strength::identical);
    }

    {
        text::variable_weighting result = text::variable_weighting::shifted;
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [&result](text::variable_weighting weighting) {
                result = weighting;
            },
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [](std::vector<text::string> && reorderings, bool simple) {},
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};

        text::string_view sv;

        sv = "[alternate non-ignorable]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, text::variable_weighting::non_ignorable);

        sv = "[alternate shifted]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, text::variable_weighting::shifted);
    }

    {
        text::l2_weight_order result = text::l2_weight_order::forward;
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [&result](text::l2_weight_order order) { result = order; },
            [](text::detail::cp_seq_t const & suppressions) {},
            [](std::vector<text::string> && reorderings, bool simple) {},
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};

        text::string_view sv;

        sv = "[backwards 2]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, text::l2_weight_order::backward);
    }

    {
        std::vector<text::string> result;
        std::vector<text::string> const expected = {
            "space", "punct", "symbol", "currency", "digit", "Latn", "Grek",
            "Copt",  "Cyrl",  "Glag",   "Perm",     "Geor",  "Armn", "Hebr",
            "Phnx",  "Samr",  "Arab",   "Syrc",     "Mand",  "Thaa", "Nkoo",
            "Tfng",  "Ethi",  "Deva",   "Beng",     "Guru",  "Gujr", "Orya",
            "Taml",  "Telu",  "Knda",   "Mlym",     "Sinh",  "Mtei", "Sylo",
            "Saur",  "Kthi",  "Mahj",   "Shrd",     "Khoj",  "Sind", "Mult",
            "Gran",  "Newa",  "Tirh",   "Sidd",     "Modi",  "Takr", "Ahom",
            "Gonm",  "Sund",  "Brah",   "Khar",     "Bhks",  "Thai", "Laoo",
            "Tavt",  "Tibt",  "Zanb",   "Soyo",     "Marc",  "Lepc", "Phag",
            "Limb",  "Tglg",  "Hano",   "Buhd",     "Tagb",  "Bugi", "Batk",
            "Rjng",  "Kali",  "Mymr",   "Cakm",     "Khmr",  "Tale", "Talu",
            "Lana",  "Cham",  "Bali",   "Java",     "Mong",  "Olck", "Cher",
            "Osge",  "Cans",  "Ogam",   "Runr",     "Hung",  "Orkh", "Vaii",
            "Bamu",  "Bass",  "Mend",   "Adlm",     "Hang",  "Hira", "Bopo",
            "Yiii",  "Lisu",  "Plrd",   "Wara",     "Pauc",  "Hmng", "Lyci",
            "Cari",  "Lydi",  "Ital",   "Goth",     "Dsrt",  "Shaw", "Dupl",
            "Osma",  "Elba",  "Aghb",   "Sora",     "Mroo",  "Linb", "Lina",
            "Cprt",  "Sarb",  "Narb",   "Avst",     "Palm",  "Nbat", "Hatr",
            "Armi",  "Prti",  "Phli",   "Phlp",     "Mani",  "Ugar", "Xpeo",
            "Xsux",  "Egyp",  "Merc",   "Hluw",     "Tang",  "Nshu", "Hani"};
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [&result](std::vector<text::string> && reorderings, bool simple) {
                result = std::move(reorderings);
            },
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};

        text::string_view sv = "[reorder others]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, expected);
    }

    {
        std::vector<text::string> result;
        std::vector<text::string> const expected = {
            "space", "currency", "digit", "symbol", "Latn", "Grek", "Copt",
            "Cyrl",  "Glag",     "Perm",  "Geor",   "Armn", "Hebr", "Phnx",
            "Samr",  "Arab",     "Syrc",  "Mand",   "Thaa", "Nkoo", "Tfng",
            "Ethi",  "Deva",     "Beng",  "Guru",   "Gujr", "Orya", "Taml",
            "Telu",  "Knda",     "Mlym",  "Sinh",   "Mtei", "Sylo", "Saur",
            "Kthi",  "Mahj",     "Shrd",  "Khoj",   "Sind", "Mult", "Gran",
            "Newa",  "Tirh",     "Sidd",  "Modi",   "Takr", "Ahom", "Gonm",
            "Sund",  "Brah",     "Khar",  "Bhks",   "Thai", "Laoo", "Tavt",
            "Tibt",  "Zanb",     "Soyo",  "Marc",   "Lepc", "Phag", "Limb",
            "Tglg",  "Hano",     "Buhd",  "Tagb",   "Bugi", "Batk", "Rjng",
            "Kali",  "Mymr",     "Cakm",  "Khmr",   "Tale", "Talu", "Lana",
            "Cham",  "Bali",     "Java",  "Mong",   "Olck", "Cher", "Osge",
            "Cans",  "Ogam",     "Runr",  "Hung",   "Orkh", "Vaii", "Bamu",
            "Bass",  "Mend",     "Adlm",  "Hang",   "Hira", "Bopo", "Yiii",
            "Lisu",  "Plrd",     "Wara",  "Pauc",   "Hmng", "Lyci", "Cari",
            "Lydi",  "Ital",     "Goth",  "Dsrt",   "Shaw", "Dupl", "Osma",
            "Elba",  "Aghb",     "Sora",  "Mroo",   "Linb", "Lina", "Cprt",
            "Sarb",  "Narb",     "Avst",  "Palm",   "Nbat", "Hatr", "Armi",
            "Prti",  "Phli",     "Phlp",  "Mani",   "Ugar", "Xpeo", "Xsux",
            "Egyp",  "Merc",     "Hluw",  "Tang",   "Nshu", "Hani", "punct"};
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [&result](std::vector<text::string> && reorderings, bool simple) {
                result = std::move(reorderings);
            },
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};

        text::string_view sv = "[reorder symbol others punct]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, expected);
    }

    {
        std::vector<text::string> result;
        std::vector<text::string> const expected = {
            "space", "currency", "digit", "symbol", "punct", "Grek", "Latn",
            "Copt",  "Cyrl",     "Glag",  "Perm",   "Geor",  "Armn", "Hebr",
            "Phnx",  "Samr",     "Arab",  "Syrc",   "Mand",  "Thaa", "Nkoo",
            "Tfng",  "Ethi",     "Deva",  "Beng",   "Guru",  "Gujr", "Orya",
            "Taml",  "Telu",     "Knda",  "Mlym",   "Sinh",  "Mtei", "Sylo",
            "Saur",  "Kthi",     "Mahj",  "Shrd",   "Khoj",  "Sind", "Mult",
            "Gran",  "Newa",     "Tirh",  "Sidd",   "Modi",  "Takr", "Ahom",
            "Gonm",  "Sund",     "Brah",  "Khar",   "Bhks",  "Thai", "Laoo",
            "Tavt",  "Tibt",     "Zanb",  "Soyo",   "Marc",  "Lepc", "Phag",
            "Limb",  "Tglg",     "Hano",  "Buhd",   "Tagb",  "Bugi", "Batk",
            "Rjng",  "Kali",     "Mymr",  "Cakm",   "Khmr",  "Tale", "Talu",
            "Lana",  "Cham",     "Bali",  "Java",   "Mong",  "Olck", "Cher",
            "Osge",  "Cans",     "Ogam",  "Runr",   "Hung",  "Orkh", "Vaii",
            "Bamu",  "Bass",     "Mend",  "Adlm",   "Hang",  "Hira", "Bopo",
            "Yiii",  "Lisu",     "Plrd",  "Wara",   "Pauc",  "Hmng", "Lyci",
            "Cari",  "Lydi",     "Ital",  "Goth",   "Dsrt",  "Shaw", "Dupl",
            "Osma",  "Elba",     "Aghb",  "Sora",   "Mroo",  "Linb", "Lina",
            "Cprt",  "Sarb",     "Narb",  "Avst",   "Palm",  "Nbat", "Hatr",
            "Armi",  "Prti",     "Phli",  "Phlp",   "Mani",  "Ugar", "Xpeo",
            "Xsux",  "Egyp",     "Merc",  "Hluw",   "Tang",  "Nshu", "Hani"};
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [&result](std::vector<text::string> && reorderings, bool simple) {
                result = std::move(reorderings);
            },
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};

        text::string_view sv = "[reorder symbol punct Grek]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(result, expected);
    }

    {
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [](std::vector<text::string> && reorderings, bool simple) {},
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};
        text::string_view sv = "[reorder Sinh Mtei Sylo Saur]";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
    }

    {
        bool reordering_is_simple = true;
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [&](std::vector<text::string> && reorderings, bool simple) {
                reordering_is_simple = simple;
            },
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};
        text::string_view sv =
            "[reorder Sinh]"; // Shares lead byte with other scripts.
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_FALSE(reordering_is_simple);
    }

    {
        bool reordering_is_simple = false;
        text::detail::collation_tailoring_interface callbacks = {
            [](text::detail::cp_seq_t const & reset_, bool before_) {},
            [](text::detail::relation_t const & rel) {},
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [&](std::vector<text::string> && reorderings, bool simple) {
                reordering_is_simple = simple;
            },
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};
        text::string_view sv = "[reorder Grek]"; // Has its own lead byte.
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_FALSE(reordering_is_simple);
    }
}

TEST(parser, rules)
{
    {
        text::detail::cp_seq_t reset_result;
        bool before_result = true;
        text::detail::relation_t relation_result;
        text::detail::cp_seq_t const abc = {'a', 'b', 'c'};
        text::detail::cp_seq_t const xyz = {'x', 'y', 'z'};
        text::detail::cp_seq_t const _123 = {'1', '2', '3'};
        text::detail::cp_seq_t const foo = {'f', 'o', 'o'};
        text::detail::cp_seq_t const last_regular = {
            text::detail::last_regular};

        text::detail::collation_tailoring_interface callbacks = {
            [&](text::detail::cp_seq_t const & reset_, bool before_) {
                reset_result = reset_;
                before_result = before_;
            },
            [&](text::detail::relation_t const & rel) {
                relation_result = rel;
            },
            [](text::collation_strength strength) {},
            [](text::variable_weighting weighting) {},
            [](text::l2_weight_order order) {},
            [](text::detail::cp_seq_t const & suppressions) {},
            [](std::vector<text::string> && reorderings, bool simple) {},
            [](text::string const & s) { std::cout << s; },
            [](text::string const & s) { std::cout << s; }};

        text::string_view sv;

        // Testing operators and before/after.

        sv = "& a = b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(relation_result.op_, text::detail::token_kind::equal);

        sv = "& [before 1] a = b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, true);
        EXPECT_EQ(relation_result.op_, text::detail::token_kind::equal);

        sv = "& a < b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);

        sv = "& [before 1] a < b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, true);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);

        sv = "& a << b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::secondary_before);

        sv = "& [before 2] a << b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, true);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::secondary_before);

        sv = "& a <<< b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::tertiary_before);

        sv = "& [before 3] a <<< b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, true);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::tertiary_before);

        sv = "& a <<<< b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::quaternary_before);


        sv = "& a =* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(relation_result.op_, text::detail::token_kind::equal);

        sv = "& [before 1] a =* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, true);
        EXPECT_EQ(relation_result.op_, text::detail::token_kind::equal);

        sv = "& a <* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);

        sv = "& [before 1] a <* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, true);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);

        sv = "& a <<* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::secondary_before);

        sv = "& [before 2] a <<* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, true);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::secondary_before);

        sv = "& a <<<* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::tertiary_before);

        sv = "& [before 3] a <<<* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, true);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::tertiary_before);

        sv = "& a <<<<* b";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::quaternary_before);


        // Testing prefixes and extensions.

        sv = "& abc < xyz | 123 / foo";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(reset_result, abc);
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);
        EXPECT_EQ(relation_result.cps_, xyz);
        EXPECT_EQ(relation_result.prefix_and_extension_.prefix_, _123);
        EXPECT_EQ(relation_result.prefix_and_extension_.extension_, foo);

        sv = "& abc < xyz / foo | 123";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(reset_result, abc);
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);
        EXPECT_EQ(relation_result.cps_, xyz);
        EXPECT_EQ(relation_result.prefix_and_extension_.prefix_, _123);
        EXPECT_EQ(relation_result.prefix_and_extension_.extension_, foo);

        sv = "& abc < xyz | 123";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(reset_result, abc);
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);
        EXPECT_EQ(relation_result.cps_, xyz);
        EXPECT_EQ(relation_result.prefix_and_extension_.prefix_, _123);
        EXPECT_EQ(
            relation_result.prefix_and_extension_.extension_,
            text::detail::optional_cp_seq_t());

        sv = "& abc < xyz / foo";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(reset_result, abc);
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);
        EXPECT_EQ(relation_result.cps_, xyz);
        EXPECT_EQ(
            relation_result.prefix_and_extension_.prefix_,
            text::detail::optional_cp_seq_t());
        EXPECT_EQ(relation_result.prefix_and_extension_.extension_, foo);

        sv = "& abc < xyz";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(reset_result, abc);
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);
        EXPECT_EQ(relation_result.cps_, xyz);
        EXPECT_EQ(
            relation_result.prefix_and_extension_.prefix_,
            text::detail::optional_cp_seq_t());
        EXPECT_EQ(
            relation_result.prefix_and_extension_.extension_,
            text::detail::optional_cp_seq_t());


        // Testing full rules.

        sv = "& abc < xyz | 123 / foo << foo";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(reset_result, abc);
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::secondary_before);
        EXPECT_EQ(relation_result.cps_, foo);
        EXPECT_EQ(
            relation_result.prefix_and_extension_.prefix_,
            text::detail::optional_cp_seq_t());
        EXPECT_EQ(
            relation_result.prefix_and_extension_.extension_,
            text::detail::optional_cp_seq_t());

        sv = "& [last regular] < bar < xyz | foo / 123";
        EXPECT_NO_THROW(text::detail::parse(
            sv.begin(), sv.end(), callbacks, "<test-string>"));
        EXPECT_EQ(reset_result, last_regular);
        EXPECT_EQ(before_result, false);
        EXPECT_EQ(
            relation_result.op_, text::detail::token_kind::primary_before);
        EXPECT_EQ(relation_result.cps_, xyz);
        EXPECT_EQ(relation_result.prefix_and_extension_.prefix_, foo);
        EXPECT_EQ(relation_result.prefix_and_extension_.extension_, _123);
    }
}
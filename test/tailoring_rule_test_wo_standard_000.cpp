
// Warning! This file is autogenerated.
#include <boost/text/collation_table.hpp>
#include <boost/text/collate.hpp>
#include <boost/text/data/all.hpp>

#ifndef LIMIT_TESTING_FOR_CI
#include <boost/text/save_load_table.hpp>

#include <boost/filesystem.hpp>
#endif

#include <gtest/gtest.h>

using namespace boost::text;

auto const error = [](string const & s) { std::cout << s; };
auto const warning = [](string const & s) {};

collation_table make_save_load_table()
{
#ifdef LIMIT_TESTING_FOR_CI
    string const table_str(data::wo::standard_collation_tailoring());
    return tailored_collation_table(
        table_str,
        "wo::standard_collation_tailoring()", error, warning);
#else
    if (!exists(boost::filesystem::path("wo_standard.table"))) {
        string const table_str(data::wo::standard_collation_tailoring());
        collation_table table = tailored_collation_table(
            table_str,
            "wo::standard_collation_tailoring()", error, warning);
        save_table(table, "wo_standard.table.0");
        boost::filesystem::rename("wo_standard.table.0", "wo_standard.table");
    }
    return load_table("wo_standard.table");
#endif
}
collation_table const & table()
{
    static collation_table retval = make_save_load_table();
    return retval;
}
TEST(tailoring, wo_standard_000_001)
{
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x0041);
    auto const rel = std::vector<uint32_t>(1, 0x00e0);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::primary),
        -1);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x00e0);
    auto const rel = std::vector<uint32_t>(1, 0x00c0);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::tertiary),
        -1);
    // equal to preceeding cps at next-lower strength
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::secondary),
        0);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x0045);
    auto const rel = std::vector<uint32_t>(1, 0x00e9);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::primary),
        -1);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x00e9);
    auto const rel = std::vector<uint32_t>(1, 0x00c9);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::tertiary),
        -1);
    // equal to preceeding cps at next-lower strength
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::secondary),
        0);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x00c9);
    auto const rel = std::vector<uint32_t>(1, 0x00eb);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::primary),
        -1);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x00eb);
    auto const rel = std::vector<uint32_t>(1, 0x00cb);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::tertiary),
        -1);
    // equal to preceeding cps at next-lower strength
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::secondary),
        0);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x004e);
    auto const rel = std::vector<uint32_t>(1, 0x00f1);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::primary),
        -1);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x00f1);
    auto const rel = std::vector<uint32_t>(1, 0x00d1);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::tertiary),
        -1);
    // equal to preceeding cps at next-lower strength
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::secondary),
        0);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x00d1);
    auto const rel = std::vector<uint32_t>(1, 0x014b);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::primary),
        -1);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x014b);
    auto const rel = std::vector<uint32_t>(1, 0x014a);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::tertiary),
        -1);
    // equal to preceeding cps at next-lower strength
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::secondary),
        0);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x004f);
    auto const rel = std::vector<uint32_t>(1, 0x00f3);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::primary),
        -1);
    }
    {
    // greater than (or equal to, for =) preceeding cps
    auto const res = std::vector<uint32_t>(1, 0x00f3);
    auto const rel = std::vector<uint32_t>(1, 0x00d3);
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::tertiary),
        -1);
    // equal to preceeding cps at next-lower strength
    EXPECT_EQ(collate(
        res.begin(), res.end(),
        rel.begin(), rel.end(),
        table(), collation_strength::secondary),
        0);
    }
}

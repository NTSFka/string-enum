/* ************************************************************************ */
/* This file is part of the string-enum library                             */
/* Copyright (c) 2026 Jiri Fatka                                            */
/* ************************************************************************ */

#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "ntsfka/string-enum.hpp"

/* ************************************************************************ */

using namespace ntsfka::string_enum;

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

enum class test_enum1 : std::uint8_t
{
    VALUE1 = 1,
    VALUE2 = 2,
    VALUE3 = 3,
    VALUE4 = 4,
};

/* ************************************************************************ */

enum class test_enum2 : std::uint8_t
{
    VALUE1 = 1,
    VALUE2 = 2,
    VALUE3 = 3,
    VALUE4 = 4,
};

/* ************************************************************************ */

} // namespace

/* ************************************************************************ */

template<>
struct ntsfka::string_enum::convertor<test_enum1>
{
    [[nodiscard]]
    static constexpr auto to_string(const test_enum1 value) -> std::string_view
    {
        switch (value)
        {
        case test_enum1::VALUE1: return "VALUE1";
        case test_enum1::VALUE2: return "VALUE2";
        case test_enum1::VALUE3: return "VALUE3";
        case test_enum1::VALUE4: return "VALUE4";
        default: return "?";
        }
    }
};

/* ************************************************************************ */

template<>
struct ntsfka::string_enum::convertor<test_enum2, tag_print_t>
{
    [[nodiscard]]
    static constexpr auto to_string(const test_enum2 value) -> std::string_view
    {
        switch (value)
        {
        case test_enum2::VALUE1: return "V1";
        case test_enum2::VALUE2: return "V2";
        case test_enum2::VALUE3: return "V3";
        case test_enum2::VALUE4: return "V4";
        default: return "?";
        }
    }
};

/* ************************************************************************ */

static_assert(printable_enum<test_enum1>);

/* ************************************************************************ */

TEST_CASE("string_enum format", "[unit]")
{
    struct data
    {
        test_enum1 input;
        std::string_view expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = test_enum1::VALUE1, .expected = "VALUE1"},
        data{.input = test_enum1::VALUE2, .expected = "VALUE2"},
        data{.input = test_enum1::VALUE3, .expected = "VALUE3"},
        data{.input = test_enum1::VALUE4, .expected = "VALUE4"}
    );

    CHECK(std::format("{}", value) == expected);
}

/* ************************************************************************ */

TEST_CASE("string_enum format invalid", "[unit]")
{
    CHECK(std::format("{}", static_cast<test_enum1>(12)) == "?");
}

/* ************************************************************************ */

TEST_CASE("string_enum ostream", "[unit]")
{
    struct data
    {
        test_enum1 input;
        std::string_view expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = test_enum1::VALUE1, .expected = "VALUE1"},
        data{.input = test_enum1::VALUE2, .expected = "VALUE2"},
        data{.input = test_enum1::VALUE3, .expected = "VALUE3"},
        data{.input = test_enum1::VALUE4, .expected = "VALUE4"}
    );

    auto oss = std::ostringstream();
    oss << value;

    CHECK(oss.str() == expected);
}

/* ************************************************************************ */

TEST_CASE("string_enum ostream invalid", "[unit]")
{
    auto oss = std::ostringstream();
    oss << static_cast<test_enum1>(12); // NOLINT

    CHECK(oss.str() == "?");
}

/* ************************************************************************ */

TEST_CASE("string_enum format - print tag", "[unit]")
{
    struct data
    {
        test_enum2 input;
        std::string_view expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = test_enum2::VALUE1, .expected = "V1"},
        data{.input = test_enum2::VALUE2, .expected = "V2"},
        data{.input = test_enum2::VALUE3, .expected = "V3"},
        data{.input = test_enum2::VALUE4, .expected = "V4"}
    );

    CHECK(std::format("{}", value) == expected);
}

/* ************************************************************************ */

TEST_CASE("string_enum format invalid - print tag", "[unit]")
{
    CHECK(std::format("{}", static_cast<test_enum2>(12)) == "?");
}

/* ************************************************************************ */

TEST_CASE("string_enum ostream - print tag", "[unit]")
{
    struct data
    {
        test_enum2 input;
        std::string_view expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = test_enum2::VALUE1, .expected = "V1"},
        data{.input = test_enum2::VALUE2, .expected = "V2"},
        data{.input = test_enum2::VALUE3, .expected = "V3"},
        data{.input = test_enum2::VALUE4, .expected = "V4"}
    );

    auto oss = std::ostringstream();
    oss << value;

    CHECK(oss.str() == expected);
}

/* ************************************************************************ */

TEST_CASE("string_enum ostream invalid - print tag", "[unit]")
{
    auto oss = std::ostringstream();
    oss << static_cast<test_enum2>(12); // NOLINT

    CHECK(oss.str() == "?");
}

/* ************************************************************************ */

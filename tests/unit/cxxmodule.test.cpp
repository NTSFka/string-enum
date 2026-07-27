/* ************************************************************************ */
/* This file is part of the string-enum library                             */
/* Copyright (c) 2026 Jiri Fatka                                            */
/* ************************************************************************ */

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

import ntsfka.string_enum;

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

    [[nodiscard]]
    static constexpr auto try_from_string(const std::string_view value) -> std::optional<test_enum1>
    {
        if (value == "VALUE1")
            return test_enum1::VALUE1;

        if (value == "VALUE2")
            return test_enum1::VALUE2;

        if (value == "VALUE3")
            return test_enum1::VALUE3;

        if (value == "VALUE4")
            return test_enum1::VALUE4;

        return std::nullopt;
    }
};

/* ************************************************************************ */

static_assert(parseable_enum<test_enum1>);
static_assert(printable_enum<test_enum1>);
static_assert(!parseable_enum<test_enum2>);
static_assert(!printable_enum<test_enum2>);

/* ************************************************************************ */

TEST_CASE("cxxmodule: string_enum to_string", "[unit][cxxmodule]")
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

    CHECK(ntsfka::string_enum::to_string(value) == expected);
}

/* ************************************************************************ */

TEST_CASE("cxxmodule: string_enum to_string invalid", "[unit][cxxmodule]")
{
    CHECK(ntsfka::string_enum::to_string(static_cast<test_enum1>(12)) == "?");
}

/* ************************************************************************ */

TEST_CASE("cxxmodule: string_enum try_from_string", "[unit][cxxmodule]")
{
    struct data
    {
        std::string_view input;
        std::optional<test_enum1> expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = "VALUE1", .expected = test_enum1::VALUE1},
        data{.input = "VALUE2", .expected = test_enum1::VALUE2},
        data{.input = "VALUE3", .expected = test_enum1::VALUE3},
        data{.input = "VALUE4", .expected = test_enum1::VALUE4},
        data{.input = "VALUE5", .expected = std::nullopt}
    );

    CHECK(try_from_string<test_enum1>(value) == expected);
}

/* ************************************************************************ */

TEST_CASE("cxxmodule: string_enum from_string", "[unit][cxxmodule]")
{
    struct data
    {
        std::string_view input;
        test_enum1 expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = "VALUE1", .expected = test_enum1::VALUE1},
        data{.input = "VALUE2", .expected = test_enum1::VALUE2},
        data{.input = "VALUE3", .expected = test_enum1::VALUE3},
        data{.input = "VALUE4", .expected = test_enum1::VALUE4}
    );

    CHECK(from_string<test_enum1>(value) == expected);
}

/* ************************************************************************ */

TEST_CASE("cxxmodule: string_enum from_string invalid", "[unit][cxxmodule]")
{
    const auto value = GENERATE("VALUE8", "VALUE9", "VALUE12", "VALUE41");

    CHECK_THROWS_AS(from_string<test_enum1>(value), std::invalid_argument);
}

/* ************************************************************************ */

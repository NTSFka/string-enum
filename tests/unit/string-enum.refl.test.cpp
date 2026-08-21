/* ************************************************************************ */
/* This file is part of the string-enum library                             */
/* Copyright (c) 2026 Jiri Fatka                                            */
/* ************************************************************************ */

#ifdef __cpp_impl_reflection

/* ************************************************************************ */

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "ntsfka/string-enum.hpp"

/* ************************************************************************ */

using namespace ntsfka::string_enum;

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

// clang-format off
inline constexpr struct {} tag_custom;
// clang-format on

/* ************************************************************************ */

// clang-format off
enum class [[=generate<>, =generate<tag_custom>]] test_enum1 : std::uint8_t
{
    VALUE1 [[=enum_value("VAL1"), =enum_value("val1", tag_custom)]] = 1,
    VALUE2 [[=enum_value("VAL2"), =enum_value("val2", tag_custom)]] = 2,
    VALUE3 [[=enum_value("VAL3"), =enum_value("val3", tag_custom)]] = 3,
    VALUE4 [[=enum_value("VAL4"), =enum_value("val4", tag_custom)]] = 4,
};
// clang-format on

/* ************************************************************************ */

enum class test_enum2 : std::uint8_t
{
};

/* ************************************************************************ */

} // namespace

/* ************************************************************************ */

static_assert(parseable_enum<test_enum1>);
static_assert(printable_enum<test_enum1>);
static_assert(parseable_enum<test_enum1, std::remove_cvref_t<decltype(tag_custom)>>);
static_assert(printable_enum<test_enum1, std::remove_cvref_t<decltype(tag_custom)>>);

static_assert(!parseable_enum<test_enum2>);
static_assert(!printable_enum<test_enum2>);

/* ************************************************************************ */

TEST_CASE("refl: string_enum to_string", "[unit]")
{
    struct data
    {
        test_enum1 input;
        std::string_view expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = test_enum1::VALUE1, .expected = "VAL1"},
        data{.input = test_enum1::VALUE2, .expected = "VAL2"},
        data{.input = test_enum1::VALUE3, .expected = "VAL3"},
        data{.input = test_enum1::VALUE4, .expected = "VAL4"}
    );

    CHECK(ntsfka::string_enum::to_string(value) == expected);
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum to_string invalid", "[unit]")
{
    CHECK(ntsfka::string_enum::to_string(static_cast<test_enum1>(12)) == "?");
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum try_from_string", "[unit]")
{
    struct data
    {
        std::string_view input;
        std::optional<test_enum1> expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = "VAL1", .expected = test_enum1::VALUE1},
        data{.input = "VAL2", .expected = test_enum1::VALUE2},
        data{.input = "VAL3", .expected = test_enum1::VALUE3},
        data{.input = "VAL4", .expected = test_enum1::VALUE4},
        data{.input = "VALUE1", .expected = std::nullopt},
        data{.input = "VALUE2", .expected = std::nullopt},
        data{.input = "VALUE3", .expected = std::nullopt},
        data{.input = "VALUE4", .expected = std::nullopt}
    );

    CHECK(try_from_string<test_enum1>(value) == expected);
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum from_string", "[unit]")
{
    struct data
    {
        std::string_view input;
        test_enum1 expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = "VAL1", .expected = test_enum1::VALUE1},
        data{.input = "VAL2", .expected = test_enum1::VALUE2},
        data{.input = "VAL3", .expected = test_enum1::VALUE3},
        data{.input = "VAL4", .expected = test_enum1::VALUE4}
    );

    CHECK(from_string<test_enum1>(value) == expected);
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum from_string invalid", "[unit]")
{
    const auto value = GENERATE("VALUE1", "VALUE8", "VALUE9", "VALUE12", "VALUE41");

    CHECK_THROWS_AS(from_string<test_enum1>(value), std::invalid_argument);
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum to_string, custom tag", "[unit]")
{
    struct data
    {
        test_enum1 input;
        std::string_view expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = test_enum1::VALUE1, .expected = "val1"},
        data{.input = test_enum1::VALUE2, .expected = "val2"},
        data{.input = test_enum1::VALUE3, .expected = "val3"},
        data{.input = test_enum1::VALUE4, .expected = "val4"}
    );

    CHECK(ntsfka::string_enum::to_string(value, tag_custom) == expected);
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum to_string invalid, custom tag", "[unit]")
{
    CHECK(ntsfka::string_enum::to_string(static_cast<test_enum1>(12), tag_custom) == "?");
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum try_from_string, custom tag", "[unit]")
{
    struct data
    {
        std::string_view input;
        std::optional<test_enum1> expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = "val1", .expected = test_enum1::VALUE1},
        data{.input = "val2", .expected = test_enum1::VALUE2},
        data{.input = "val3", .expected = test_enum1::VALUE3},
        data{.input = "val4", .expected = test_enum1::VALUE4},
        data{.input = "VAL1", .expected = std::nullopt},
        data{.input = "VAL2", .expected = std::nullopt},
        data{.input = "VAL3", .expected = std::nullopt},
        data{.input = "VAL4", .expected = std::nullopt},
        data{.input = "VALUE1", .expected = std::nullopt},
        data{.input = "VALUE2", .expected = std::nullopt},
        data{.input = "VALUE3", .expected = std::nullopt},
        data{.input = "VALUE4", .expected = std::nullopt}
    );

    CHECK(try_from_string<test_enum1>(value, tag_custom) == expected);
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum from_string, custom tag", "[unit]")
{
    struct data
    {
        std::string_view input;
        test_enum1 expected;
    };

    const auto [value, expected] = GENERATE(
        data{.input = "val1", .expected = test_enum1::VALUE1},
        data{.input = "val2", .expected = test_enum1::VALUE2},
        data{.input = "val3", .expected = test_enum1::VALUE3},
        data{.input = "val4", .expected = test_enum1::VALUE4}
    );

    CHECK(from_string<test_enum1>(value, tag_custom) == expected);
}

/* ************************************************************************ */

TEST_CASE("refl: string_enum from_string invalid, custom tag", "[unit]")
{
    const auto value = GENERATE("VALUE1", "VALUE8", "VALUE9", "VALUE12", "VALUE41", "VAL1", "VAL2", "VAL3", "VAL4");

    CHECK_THROWS_AS(from_string<test_enum1>(value, tag_custom), std::invalid_argument);
}

/* ************************************************************************ */

#endif

/* ************************************************************************ */

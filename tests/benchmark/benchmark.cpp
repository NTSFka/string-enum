/* ************************************************************************ */
/* This file is part of the string-enum library                             */
/* Copyright (c) 2026 Jiri Fatka                                            */
/* ************************************************************************ */

#include <array>
#include <iostream>
#include <random>
#include <unordered_map>

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_case_info.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/reporters/catch_reporter_registrars.hpp>
#include <catch2/reporters/catch_reporter_streaming_base.hpp>

#include "ntsfka/string-enum.hpp"

/* ************************************************************************ */

#ifndef CONFIG_ENUM_SIZE
#define CONFIG_ENUM_SIZE 16
#endif

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * Fowler-Noll-Vo hash helper.
 *
 * @tparam INTEGER Integer type.
 */
template<std::unsigned_integral INTEGER>
struct fnv;

/* ************************************************************************ */

/**
 * Fowler-Noll-Vo hash helper. Specialization for 32-bit unsigned integer type.
 */
template<>
struct fnv<std::uint32_t>
{
    static constexpr std::uint32_t basis = 2166136261U;
    static constexpr std::uint32_t prime = 16777619U;
};

/* ************************************************************************ */

/**
 * Fowler-Noll-Vo hash helper. Specialization for 64-bit unsigned integer type.
 */
template<>
struct fnv<std::uint64_t>
{
    static constexpr std::uint64_t basis = 14695981039346656037U;
    static constexpr std::uint64_t prime = 1099511628211U;
};

/* ************************************************************************ */

/**
 * Calculate FNV hash for given byte.
 *
 * @param hash Initial hash.
 * @param value Value to add to the hash.
 *
 * @tparam INTEGER Result integral type.
 * @tparam VALUE Value type.
 *
 * @return Result hash.
 */
template<std::unsigned_integral INTEGER, typename VALUE>
[[nodiscard]]
constexpr auto hash(INTEGER hash, VALUE value) noexcept -> INTEGER
{
    hash = hash * fnv<INTEGER>::prime;
    hash = hash ^ value;

    return hash;
}

/* ************************************************************************ */

/**
 * Calculate FNV hash for given byte.
 *
 * @param range The range with bytes.
 *
 * @tparam INTEGER Result integral type.
 *
 * @return Result hash.
 */
template<std::unsigned_integral INTEGER>
[[nodiscard]]
constexpr auto hash(const std::string_view range) noexcept -> INTEGER
{
    INTEGER result = fnv<INTEGER>::basis;

    for (auto value : range)
        result = hash(result, value);

    return result;
}

/* ************************************************************************ */

// #define ENUM_IMPL(type, name, value)
#define ENUM_BUILD_ENUMERATOR(type, value) ENUM_IMPL(type, ENUM_LONG_VALUE_##value, value)

#include "enum-foreach.hpp"

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define ENUM_FOREACH_SIZE(type, size) CONCAT(ENUM_FOREACH_, size)(type)

#define ENUM_FOREACH(type) ENUM_FOREACH_SIZE(type, CONFIG_ENUM_SIZE)

/* ************************************************************************ */

// NOLINTNEXTLINE
enum class test_enum
{
#define ENUM_IMPL(type, name, value) name = value,
    ENUM_FOREACH(test_enum)
#undef ENUM_IMPL
};

/* ************************************************************************ */

constexpr struct tag_manual_t
{
} tag_manual;

constexpr struct tag_hash_t
{
} tag_hash;

constexpr struct tag_map_t
{
} tag_map;

constexpr struct tag_array_t
{
} tag_array;

/* ************************************************************************ */

} // namespace

/* ************************************************************************ */

template<>
struct ntsfka::string_enum::convertor<test_enum, tag_manual_t>
{
    [[nodiscard]]
    static constexpr auto to_string(const test_enum value) -> std::string_view
    {
        switch (value)
        {
#define ENUM_IMPL(type, name, value)                                                                                   \
    case type::name: return #name;
            ENUM_FOREACH(test_enum)
#undef ENUM_IMPL
        default: return "?";
        }
    }

    [[nodiscard]]
    static constexpr auto try_from_string(const std::string_view value) -> std::optional<test_enum>
    {
#define ENUM_IMPL(type, name, val)                                                                                     \
    if (value == #name)                                                                                                \
        return type::name;

        ENUM_FOREACH(test_enum)
#undef ENUM_IMPL

        return std::nullopt;
    }
};

/* ************************************************************************ */

template<>
struct ntsfka::string_enum::convertor<test_enum, tag_hash_t>
{
    [[nodiscard]]
    static constexpr auto to_string(const test_enum value) -> std::string_view
    {
        switch (value)
        {
#define ENUM_IMPL(type, name, value)                                                                                   \
    case type::name: return #name;
            ENUM_FOREACH(test_enum)
#undef ENUM_IMPL
        default: return "?";
        }
    }

    [[nodiscard]]
    static constexpr auto try_from_string(const std::string_view value) -> std::optional<test_enum>
    {
        switch (hash<std::uint32_t>(value))
        {
#define ENUM_IMPL(type, name, value)                                                                                   \
    case hash<std::uint32_t>(#name): return type::name;
            ENUM_FOREACH(test_enum)
#undef ENUM_IMPL
        default: return std::nullopt;
        }
    }
};

/* ************************************************************************ */

template<>
struct ntsfka::string_enum::convertor<test_enum, tag_map_t>
{
    static const inline std::unordered_map<std::string_view, test_enum> mappingFrom = {
#define ENUM_IMPL(type, name, value) {#name, type::name},
        ENUM_FOREACH(test_enum)
#undef ENUM_IMPL
    };

    static const inline std::unordered_map<test_enum, std::string_view> mappingTo = {
#define ENUM_IMPL(type, name, value) {type::name, #name},
        ENUM_FOREACH(test_enum)
#undef ENUM_IMPL
    };

    [[nodiscard]]
    static auto to_string(const test_enum value) -> std::string_view
    {
        if (const auto rec = mappingTo.find(value); rec != mappingTo.end())
            return rec->second;

        return "?";
    }

    [[nodiscard]]
    static auto try_from_string(const std::string_view value) -> std::optional<test_enum>
    {
        if (const auto rec = mappingFrom.find(value); rec != mappingFrom.end())
            return rec->second;

        return std::nullopt;
    }
};

/* ************************************************************************ */

template<typename ENUM>
struct pair
{
    ENUM value;
    std::string_view name;
};

/* ************************************************************************ */

template<>
struct ntsfka::string_enum::convertor<test_enum, tag_array_t>
{
    static constexpr auto mapping = std::array<pair<test_enum>, CONFIG_ENUM_SIZE>{
#define ENUM_IMPL(type, name, value) pair<type>{type::name, #name},
        ENUM_FOREACH(test_enum)
#undef ENUM_IMPL
    };

    [[nodiscard]]
    static auto to_string(const test_enum value) -> std::string_view
    {
        for (const auto& pair : mapping)
        {
            if (pair.value == value)
                return pair.name;
        }

        return {};
    }

    [[nodiscard]]
    static auto try_from_string(const std::string_view value) -> std::optional<test_enum>
    {
        for (const auto& pair : mapping)
        {
            if (pair.name == value)
                return pair.value;
        }

        return std::nullopt;
    }
};

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

class CsvReporter : public Catch::StreamingReporterBase
{
public:
    using StreamingReporterBase::StreamingReporterBase; // NOLINT

    [[maybe_unused]]
    static auto getDescription() -> std::string // NOLINT
    {
        return "CSV benchmark reporter";
    }

    void testCaseStarting(Catch::TestCaseInfo const& testInfo) override
    {
        m_currentCase = testInfo.name;
    }

    void testCaseEnded(Catch::TestCaseStats const& testCaseStats) override
    {
        m_currentCase.clear();
    }

    void benchmarkEnded(Catch::BenchmarkStats<> const& benchmarkStats) override
    {
        std::cout << CONFIG_ENUM_SIZE << ",";
        std::cout << m_currentCase << ",";
        std::cout << benchmarkStats.info.name << ",";
        std::cout << benchmarkStats.mean.point.count() << ",";
        std::cout << benchmarkStats.standardDeviation.point.count();
        std::cout << '\n';
    }

private:
    std::string m_currentCase;
};

/* ************************************************************************ */

} // namespace

CATCH_REGISTER_REPORTER("csv", CsvReporter)

/* ************************************************************************ */

TEST_CASE("from_string", "[!benchmark]")
{
    auto rand_dev = std::random_device();
    auto gen      = std::mt19937(rand_dev());

    constexpr auto values = std::array<std::string_view, CONFIG_ENUM_SIZE>{
#define ENUM_IMPL(type, name, value) std::string_view(#name),
        ENUM_FOREACH(a)
#undef ENUM_IMPL
    };

    auto dist = std::uniform_int_distribution<std::size_t>(0, values.size() - 1);

    BENCHMARK("manual")
    {
        const auto index = dist(gen);
        const auto value = values.at(index);

        return ntsfka::string_enum::from_string<test_enum>(value, tag_manual);
    };

    BENCHMARK("hash")
    {
        const auto index = dist(gen);
        const auto value = values.at(index);

        return ntsfka::string_enum::from_string<test_enum>(value, tag_hash);
    };

    BENCHMARK("map")
    {
        const auto index = dist(gen);
        const auto value = values.at(index);

        return ntsfka::string_enum::from_string<test_enum>(value, tag_map);
    };

    BENCHMARK("array")
    {
        const auto index = dist(gen);
        const auto value = values.at(index);

        return ntsfka::string_enum::from_string<test_enum>(value, tag_array);
    };
}

/* ************************************************************************ */

TEST_CASE("to_string", "[!benchmark]")
{
    auto rand_dev = std::random_device();
    auto gen      = std::mt19937(rand_dev());

    constexpr auto values = std::array<test_enum, CONFIG_ENUM_SIZE>{
#define ENUM_IMPL(type, name, value) type::name,
        ENUM_FOREACH(test_enum)
#undef ENUM_IMPL
    };

    auto dist = std::uniform_int_distribution<std::size_t>(0, values.size() - 1);

    // 'hash' uses same switch
    BENCHMARK("switch")
    {
        const auto index = dist(gen);
        const auto value = values.at(index);

        return ntsfka::string_enum::to_string<test_enum>(value, tag_manual);
    };

    BENCHMARK("map")
    {
        const auto index = dist(gen);
        const auto value = values.at(index);

        return ntsfka::string_enum::to_string<test_enum>(value, tag_map);
    };

    BENCHMARK("array")
    {
        const auto index = dist(gen);
        const auto value = values.at(index);

        return ntsfka::string_enum::to_string<test_enum>(value, tag_array);
    };
}

/* ************************************************************************ */

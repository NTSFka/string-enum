/* ************************************************************************ */
/* This file is part of the string-enum library                             */
/* Copyright (c) 2026 Jiri Fatka                                            */
/* ************************************************************************ */

#ifndef NTSFKA_STRING_ENUM_STRING_ENUM_HPP
#define NTSFKA_STRING_ENUM_STRING_ENUM_HPP

/* ************************************************************************ */

// C++
#include <format>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <type_traits>

/* ************************************************************************ */

namespace ntsfka::string_enum {

/* ************************************************************************ */

/**
 * The default tag.
 */
// clang-format off
inline constexpr struct tag_default_t {} tag_default;
// clang-format on

/* ************************************************************************ */

/**
 * The default implementation of string-enum conversion.
 *
 * The implementation explicitly deletes the conversion functions and in some way offers the functions
 * signature for custom implementation.
 *
 * @tparam ENUM the enumeration type to convert
 * @tparam TAG the tag type used to select the conversion implementation
 */
template<typename ENUM, typename TAG = tag_default_t>
    requires std::is_enum_v<ENUM>
struct convertor
{
    /**
     * Convert enum value to string.
     *
     * @param value the enum value to convert
     *
     * @return the string view representation of the enum value.
     */
    [[nodiscard]]
    static auto to_string(const ENUM value) noexcept -> std::string_view = delete;

    /**
     * Try to convert string to enum value.
     *
     * @param value the string value to convert
     *
     * @return the enum value if conversion is successful, otherwise std::nullopt
     */
    [[nodiscard]]
    static auto try_from_string(std::string_view value) noexcept -> std::optional<ENUM> = delete;
};

/* ************************************************************************ */

/**
 * A concept for enums that have defined convertor for parsing.
 *
 * The ENUM must be enumeration type and a convertor must be defined for a given `TAG` and
 * the convertor must have `try_from_string` function.
 *
 * @tparam ENUM the tested enumeration type
 * @tparam TAG the tag type used to select the conversion implementation
 */
template<typename ENUM, typename TAG = tag_default_t>
concept parseable_enum = std::is_enum_v<ENUM> and requires(const ENUM value, const std::string_view str) {
    { convertor<ENUM, TAG>::try_from_string(str) } -> std::convertible_to<std::optional<ENUM>>;
};

/* ************************************************************************ */

/**
 * A concept for enums that have defined convertor for printing.
 *
 * The `ENUM` must be enumeration type and a convertor must be defined for a given `TAG` and
 * the convertor must have `to_string` function.
 *
 * @tparam ENUM the tested enumeration type
 * @tparam TAG the tag type used to select the conversion implementation
 */
template<typename ENUM, typename TAG = tag_default_t>
concept printable_enum = std::is_enum_v<ENUM> and requires(const ENUM value, const std::string_view str) {
    { convertor<ENUM, TAG>::to_string(value) } -> std::convertible_to<std::string_view>;
};

/* ************************************************************************ */

/**
 * The exception type for failed conversion.
 *
 * @tparam ENUM the enumeration type to convert
 * @tparam TAG the tag type used to select the conversion implementation
 */
template<typename ENUM, typename TAG = tag_default_t>
class from_string_failed : public std::invalid_argument
{
public:
    // Ctors & Dtors

    explicit constexpr from_string_failed(const std::string_view value, const TAG tag) noexcept
        : std::invalid_argument(format("unable to convert value '{}' to enum", value))
        , m_value(value)
        , m_tag(tag)
    {
        // Nothing to do
    }

private:
    // Data members

    std::string m_value;
    TAG m_tag;
};

/* ************************************************************************ */

/**
 * Utility function to convert enum to string view.
 *
 * @tparam ENUM the enumeration type to convert
 * @tparam TAG the tag type used to select the conversion implementation
 *
 * @param value the enum value to convert
 * @param tag the tag value to select the conversion implementation
 *
 * @return the string view representation of the enum value for given tag.
 */
template<typename ENUM, typename TAG = tag_default_t>
    requires printable_enum<ENUM, TAG>
[[nodiscard]]
constexpr auto to_string(const ENUM value, [[maybe_unused]] const TAG tag = tag_default) noexcept -> std::string_view
{
    return convertor<ENUM, TAG>::to_string(value);
}

/* ************************************************************************ */

/**
 * Try to convert string to enum value.
 *
 * @tparam ENUM the enumeration type to convert
 * @tparam TAG the tag type used to select the conversion implementation
 *
 * @param value the string value to convert
 * @param tag the tag value to select the conversion implementation
 *
 * @return the enum value if conversion is successful, otherwise std::nullopt
 */
template<typename ENUM, typename TAG = tag_default_t>
    requires parseable_enum<ENUM, TAG>
[[nodiscard]]
constexpr auto try_from_string(const std::string_view value, [[maybe_unused]] const TAG tag = tag_default) noexcept
    -> std::optional<ENUM>
{
    return convertor<ENUM, TAG>::try_from_string(value);
}

/* ************************************************************************ */

/**
 * Convert string to enum value.
 *
 * @tparam ENUM the enumeration type to convert
 * @tparam TAG the tag type used to select the conversion implementation
 *
 * @param value the string value to convert
 * @param tag the tag value to select the conversion implementation
 *
 * @throws from_string_failed the value cannot be converted to enum
 */
template<typename ENUM, typename TAG = tag_default_t>
    requires parseable_enum<ENUM, TAG>
[[nodiscard]]
constexpr auto from_string(const std::string_view value, const TAG tag = tag_default) -> ENUM
{
    if (const auto res = try_from_string<ENUM, TAG>(value, tag); res.has_value())
        return res.value();

    throw from_string_failed<ENUM, TAG>(value, tag);
}

/* ************************************************************************ */

} // namespace ntsfka::string_enum

/* ************************************************************************ */

#endif

/* ************************************************************************ */

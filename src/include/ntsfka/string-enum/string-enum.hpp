/* ************************************************************************ */
/* This file is part of the string-enum library                             */
/* Copyright (c) 2026 Jiri Fatka                                            */
/* ************************************************************************ */

#ifndef NTSFKA_STRING_ENUM_STRING_ENUM_HPP
#define NTSFKA_STRING_ENUM_STRING_ENUM_HPP

/* ************************************************************************ */

// C++
#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <type_traits>

#ifdef __cpp_impl_reflection
#include <meta>
#endif

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

#ifdef __cpp_impl_reflection

/* ************************************************************************ */

/**
 * Annotation for generating convertor for enum and given tag.
 *
 * @tparam TAG The tag type the generate code for.
 */
template<typename TAG = tag_default_t>
struct generate_t
{
};

/* ************************************************************************ */

/**
 * Annotation for generating convertor for enum and given tag.
 *
 * @tparam TAG The tag type the generate code for.
 */
template<auto TAG = tag_default>
inline constexpr generate_t<std::remove_cvref_t<decltype(TAG)>> generate = {};

/* ************************************************************************ */

/**
 * Return if the enum has generate annotation.
 *
 * @tparam ENUM The tested enum.
 * @tparam TAG The tag type the generate code for.
 *
 * @return Annotation exists.
 */
template<typename ENUM, typename TAG = tag_default_t>
consteval auto has_generate() -> bool
{
    return !std::meta::annotations_of_with_type(^^ENUM, ^^generate_t<TAG>).empty();
}

/* ************************************************************************ */

/**
 * Annotation that defines enumeration value for specified tag.
 *
 * @tparam N Length of the value
 * @tparam TAG The tag type the generate value for.
 */
template<std::size_t N, typename TAG = tag_default_t>
class enum_value
{
public:
    // Ctors & Dtors

    explicit consteval enum_value(const char (&str)[N], [[maybe_unused]] const TAG tag = tag_default) noexcept
    {
        std::ranges::copy(str, value.begin());
    }

public:
    // Accessors

    /**
     * Return stored value as string_view.
     */
    [[nodiscard]]
    consteval auto str() const noexcept -> std::string_view
    {
        return {value.data(), N - 1};
    }

public:
    // Data Members (must be public for structural type)

    std::array<char, N> value;
};

/* ************************************************************************ */

/**
 * Extract enumeration value from annotation. If no annotation is found, returns the enumerator name.
 *
 * @tparam ENUMERATOR enumerator reflection info
 * @tparam TAG The tag type the value return for.
 *
 * @return The enumeration value
 */
template<std::meta::info ENUMERATOR, typename TAG = tag_default_t>
[[nodiscard]]
consteval auto get_enumeration_value() -> std::string_view
{
    constexpr auto enumerator = ENUMERATOR;

    static_assert(std::meta::is_enumerator(enumerator));

    static constexpr auto annotations = std::define_static_array(std::meta::annotations_of(enumerator));

    // Foreach annotations manually, ranges filter cannot be used because lambda is not usable in consteval because
    // the argument is not constexpr
    template for (constexpr auto annotation : annotations)
    {
        constexpr auto type = std::meta::type_of(annotation);

        // The enum_value is template and real type is dependent on value length.
        if constexpr (std::meta::template_of(type) == ^^enum_value)
        {
            // Extract size from template parameter and use it to extract annotation object
            constexpr auto size = std::meta::constant_of(std::meta::template_arguments_of(type)[0]);

            // Only for given tags
            if constexpr (std::meta::template_arguments_of(type)[1] == ^^TAG)
            {
                // Store annotation object at static storage so it outlive the function call
                static constexpr auto value = std::meta::extract<enum_value<([:size:]), TAG>>(annotation);

                return value.str();
            }
        }
    }

    // No annotation found, return the enumerator name
    return std::meta::identifier_of(enumerator);
}

/* ************************************************************************ */

#endif

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
#ifdef __cpp_impl_reflection
    /**
     * Convert enum value to string.
     *
     * @param value the enum value to convert
     *
     * @return the string view representation of the enum value.
     */
    [[nodiscard]]
    static auto to_string(const ENUM value) noexcept -> std::string_view
        requires(has_generate<ENUM, TAG>())
    {
        constexpr static auto enumerators = std::define_static_array(enumerators_of(^^ENUM));

        template for (constexpr auto enumerator : enumerators)
        {
            if (value == [:enumerator:])
                return get_enumeration_value<enumerator, TAG>();
        }

        return "?";
    }
#endif

    /**
     * Convert enum value to string.
     *
     * @param value the enum value to convert
     *
     * @return the string view representation of the enum value.
     */
    [[nodiscard]]
    static auto to_string(ENUM value) noexcept -> std::string_view = delete;

#ifdef __cpp_impl_reflection
    /**
     * Try to convert string to enum value.
     *
     * @param value the string value to convert
     *
     * @return the enum value if conversion is successful, otherwise std::nullopt
     */
    [[nodiscard]]
    static auto try_from_string(const std::string_view value) noexcept -> std::optional<ENUM>
        requires(has_generate<ENUM, TAG>())
    {
        constexpr static auto enumerators = std::define_static_array(enumerators_of(^^ENUM));

        template for (constexpr auto enumerator : enumerators)
        {
            if (value == get_enumeration_value<enumerator, TAG>())
                return [:enumerator:];
        }

        return std::nullopt;
    }
#endif

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

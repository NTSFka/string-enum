/* ************************************************************************ */
/* This file is part of the string-enum library                             */
/* Copyright (c) 2026 Jiri Fatka                                            */
/* ************************************************************************ */

#ifndef NTSFKA_STRING_ENUM_FORMAT_HPP
#define NTSFKA_STRING_ENUM_FORMAT_HPP

/* ************************************************************************ */

// C++
#include <iosfwd>

#include "string-enum.hpp"

/* ************************************************************************ */

namespace ntsfka::string_enum {

/* ************************************************************************ */

/**
 * The tag for printing enum value to ostream or format.
 */
// clang-format off
inline constexpr struct tag_print_t {} tag_print;
// clang-format on

/* ************************************************************************ */

/**
 * Default implementation of string-enum conversion for printing enum values.
 */
template<typename ENUM>
    requires std::is_enum_v<ENUM>
struct convertor<ENUM, tag_print_t> : convertor<ENUM>
{
};

/* ************************************************************************ */

} // namespace ntsfka::string_enum

/* ************************************************************************ */

/**
 * Formatter specialization for convertible enums.
 *
 * @tparam ENUM enumeration type
 * @tparam CHAR output stream character type
 */
template<ntsfka::string_enum::printable_enum<ntsfka::string_enum::tag_print_t> ENUM, typename CHAR>
struct std::formatter<ENUM, CHAR>
{
    constexpr auto parse(std::basic_format_parse_context<CHAR>& ctx) -> std::basic_format_parse_context<CHAR>::iterator
    {
        return ctx.begin();
    }

    template<std::output_iterator<CHAR> ITERATOR>
    constexpr auto format(const ENUM value, std::basic_format_context<ITERATOR, CHAR>& ctx) const -> ITERATOR
    {
        return std::format_to(ctx.out(), "{}", ntsfka::string_enum::to_string(value, ntsfka::string_enum::tag_print));
    }
};

/* ************************************************************************ */

/**
 * Write convertible enum to output stream.
 *
 * @tparam ENUM enumeration type
 * @tparam CHAR output stream character type
 *
 * @param stream the output stream
 * @param value convertible enum specification adaptor
 *
 * @return the stream
 */
template<ntsfka::string_enum::printable_enum<ntsfka::string_enum::tag_print_t> ENUM, typename CHAR>
constexpr auto operator<<(std::basic_ostream<CHAR>& stream, const ENUM value) -> std::basic_ostream<CHAR>&
{
    std::format_to(std::ostreambuf_iterator(stream), "{}", value);

    return stream;
}

/* ************************************************************************ */

#endif

/* ************************************************************************ */

/* ************************************************************************ */
/* This file is part of the string-enum library                             */
/* Copyright (c) 2026 Jiri Fatka                                            */
/* ************************************************************************ */

module;

/* ************************************************************************ */

#include "ntsfka/string-enum.hpp"

/* ************************************************************************ */

export module ntsfka.string_enum;

/* ************************************************************************ */

namespace ntsfka::string_enum {

/* ************************************************************************ */

export using ::ntsfka::string_enum::tag_default;
export using ::ntsfka::string_enum::tag_default_t;

export using ::ntsfka::string_enum::printable_enum;
export using ::ntsfka::string_enum::parseable_enum;
export using ::ntsfka::string_enum::convertor;

export using ::ntsfka::string_enum::to_string;
export using ::ntsfka::string_enum::try_from_string;
export using ::ntsfka::string_enum::from_string;

export using ::ntsfka::string_enum::tag_print;
export using ::ntsfka::string_enum::tag_print_t;

/* ************************************************************************ */

}

/* ************************************************************************ */

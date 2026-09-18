# ######################################################################## #
# This file is part of the string-enum library                             #
# Copyright (c) 2026 Jiri Fatka                                            #
# ######################################################################## #

# We need Catch2 build in at least C++17 mode (string_view support)
target_compile_features(Catch2 PUBLIC cxx_std_20)

# ######################################################################## #

add_executable(tests_unit)

target_compile_features(tests_unit PUBLIC cxx_std_20)

target_sources(tests_unit
        PRIVATE
        string-enum.test.cpp
        string-enum.refl.test.cpp
        format.test.cpp
)

if (STRING_ENUM_CXX_MODULES)
    target_sources(tests_unit
            PRIVATE
            cxxmodule.test.cpp
    )
endif ()

target_link_libraries(tests_unit
        PRIVATE
        ntsfka::string-enum
        Catch2::Catch2WithMain
)

add_coverage(tests_unit)

add_test(tests_unit tests_unit)
catch_discover_tests(tests_unit)

# ######################################################################## #

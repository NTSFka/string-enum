# ######################################################################## #
# This file is part of the string-enum library                             #
# Copyright (c) 2026 Jiri Fatka                                            #
# ######################################################################## #

option(ENABLE_COVERAGE "Enable code coverage collection" Off)

# ######################################################################## #

function (add_coverage TARGET)
    if (NOT ENABLE_COVERAGE)
        return ()
    endif ()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${TARGET} PUBLIC --coverage)
        target_link_options(${TARGET} PUBLIC --coverage)
    else ()
        message(FATAL_ERROR "Cannot setup coverage for unsupported compiler")
    endif ()
endfunction()

# ######################################################################## #

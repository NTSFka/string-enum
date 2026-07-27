# ######################################################################## #
# This file is part of the string-enum library                             #
# Copyright (c) 2026 Jiri Fatka                                            #
# ######################################################################## #

add_library(string-enum STATIC)
add_library(ntsfka::string-enum ALIAS string-enum)

target_compile_features(string-enum PUBLIC cxx_std_20)

target_sources(string-enum
        PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/ntsfka FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ntsfka/string-enum.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ntsfka/string-enum/string-enum.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ntsfka/string-enum/format.hpp
)

if (ENUM_STRING_CXX_MODULES)
    target_sources(string-enum
            PUBLIC FILE_SET CXX_MODULES FILES
            string-enum.cppm
    )
endif ()

target_include_directories(string-enum
        PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        PUBLIC "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_coverage(string-enum)

# ######################################################################## #

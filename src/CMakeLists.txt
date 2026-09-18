# ######################################################################## #
# This file is part of the string-enum library                             #
# Copyright (c) 2026 Jiri Fatka                                            #
# ######################################################################## #

add_library(string-enum_headers INTERFACE)
target_compile_features(string-enum_headers INTERFACE cxx_std_20)

target_sources(string-enum_headers
        PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/ntsfka FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ntsfka/string-enum.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ntsfka/string-enum/string-enum.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ntsfka/string-enum/format.hpp
)

target_include_directories(string-enum_headers
        INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        INTERFACE "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if (STRING_ENUM_CXX_MODULES)
    add_library(string-enum_modules)
    target_link_libraries(string-enum_modules PUBLIC string-enum_headers)

    target_sources(string-enum_modules
            PUBLIC FILE_SET CXX_MODULES FILES
            string-enum.cppm
    )

    add_library(ntsfka::string-enum ALIAS string-enum_modules)
else ()
    add_library(ntsfka::string-enum ALIAS string-enum_headers)
endif ()

add_coverage(string-enum)

# ######################################################################## #

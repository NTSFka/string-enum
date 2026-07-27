# ######################################################################## #
# This file is part of the string-enum library                             #
# Copyright (c) 2026 Jiri Fatka                                            #
# ######################################################################## #

cmake_minimum_required(VERSION 3.28)

project(ntsfka-string-enum
        VERSION 0.1
        DESCRIPTION "C++ string-enum library"
        LANGUAGES CXX
)

# ######################################################################## #

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(CPM)
include(Coverage)

# ######################################################################## #

# Allow to disable export of library's C++ modules if compiler support is broken
option(ENUM_STRING_CXX_MODULES "Enable or disable support of C++ modules" On)

# ######################################################################## #

add_subdirectory(src)

# ######################################################################## #

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    message(STATUS "string-enum: Testing enabled")
    include(CTest)

    add_subdirectory(tests)
endif ()

# ######################################################################## #

include(GNUInstallDirs)
install(TARGETS string-enum
        EXPORT string-enum-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        CXX_MODULES_BMI DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT string-enum-targets
        FILE string-enum-targets.cmake
        NAMESPACE ntsfka::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ntsfka
)

# ######################################################################## #

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/string-enumConfigVersion.cmake"
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/string-enumConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ntsfka
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/string-enumConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/string-enumConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ntsfka
)

# ######################################################################## #

# ######################################################################## #
# This file is part of the string-enum library                             #
# Copyright (c) 2026 Jiri Fatka                                            #
# ######################################################################## #

CPMAddPackage("gh:catchorg/Catch2@3.7.1")

# ######################################################################## #

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)

# ######################################################################## #

add_subdirectory(unit)
add_subdirectory(benchmark)

# ######################################################################## #

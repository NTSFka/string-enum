# ######################################################################## #
# This file is part of the string-enum library                             #
# Copyright (c) 2026 Jiri Fatka                                            #
# ######################################################################## #

add_executable(tests_benchmark)

target_sources(tests_benchmark
        PRIVATE
        benchmark.cpp
)

target_link_libraries(tests_benchmark
        PRIVATE
        ntsfka::string-enum
        Catch2::Catch2WithMain
)

# ######################################################################## #

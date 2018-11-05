cmake_minimum_required(VERSION 3.12)
project(p3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

add_library(SRC_LIB
        build_spec.c
        parser.c
        tools.c
        )

add_executable(537make main.c)
target_link_libraries(537make SRC_LIB)

add_executable(TestAll
        test_main.cpp
        test_parser.cpp)
target_link_libraries(TestAll SRC_LIB)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

set(NON_MAIN_SRC build_spec.c parser.c tools.c)

add_library(SRC_LIB
        build_spec.c
        parser.c
        tools.c
        )

add_executable(537make main.c)
target_link_libraries(537make SRC_LIB)

add_executable(TestAll test_main.cpp test_parser.cpp ${NON_MAIN_SRC})
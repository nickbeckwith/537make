set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

set(NON_MAIN_SRC build_spec.c parser.c tools.c command.c graph.c list.c)
set(TESTS test_main.cpp test_parser.cpp test_command.cpp test_build_spec.cpp test_graph.cpp)

add_executable(537make main.c ${NON_MAIN_SRC})

add_executable(TestAll ${TESTS} ${NON_MAIN_SRC})

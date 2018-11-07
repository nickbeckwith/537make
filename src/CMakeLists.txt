set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

set(NON_MAIN_SRC build_spec.c parser.c tools.c command.c command.h graph.c)
set(TESTS test_main.cpp test_parser.cpp test_command.cpp test_build_spec.cpp graph.c list.c list.h)

add_executable(537make main.c ${NON_MAIN_SRC})
target_link_libraries(537make SRC_LIB)

add_executable(TestAll ${TESTS} ${NON_MAIN_SRC})

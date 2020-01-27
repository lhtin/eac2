cmake_minimum_required(VERSION 3.15)
project(EaC2)

set(CMAKE_CXX_STANDARD 14)

include(conanbuildinfo.cmake)
conan_basic_setup()


add_executable(main main.cpp lex/RE.cpp lex/FA.cpp lex/RE2NFA.cpp lex/NFA2DFA.cpp lex/minDFA.cpp utils/utils.cpp lex/lex.cpp)

target_link_libraries(main ${CONAN_LIBS})

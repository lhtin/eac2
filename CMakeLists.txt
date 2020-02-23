cmake_minimum_required(VERSION 3.15)
project(EaC2)

set(CMAKE_CXX_STANDARD 17)

add_library(lex lex/RE.cpp lex/FA.cpp lex/NFA.cpp lex/DFA.cpp lex/minDFA.cpp lex/WrapFA.cpp)
add_library(utils utils/utils.cpp)

add_executable(test-pl0 spec/pl0/test.cpp)
target_link_libraries(test-pl0
        PRIVATE lex utils)
target_include_directories(test-pl0
        PRIVATE utils lex syntax)

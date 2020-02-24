cmake_minimum_required(VERSION 3.15)
project(EaC2)

set(CMAKE_CXX_STANDARD 17)


add_library(lex lex/RE.cpp lex/FA.cpp lex/NFA.cpp lex/DFA.cpp lex/minDFA.cpp lex/WrapFA.cpp)
add_library(utils utils/utils.cpp)

include_directories(utils src spec lex)
link_libraries(lex utils)

#### parser demo
add_executable(test-pl0 spec/pl0/test.cpp)
add_executable(test-parentheses spec/\(\)/test.cpp)
add_executable(test-01 spec/01/test.cpp)

#### test
add_executable(test-re test/test-re.cpp lex/RE.cpp)

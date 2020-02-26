cmake_minimum_required(VERSION 3.15)
project(EaC2)

set(CMAKE_CXX_STANDARD 17)

add_library(lex lex/RE.cpp lex/FA.cpp lex/NFA.cpp lex/DFA.cpp lex/minDFA.cpp lex/WrapFA.cpp)
add_library(utils utils/utils.cpp)

include_directories(utils src spec lex)
link_libraries(lex utils)

#### test
add_executable(test test/test.cpp)
add_library(testSet test/test-re.cpp test/pl0/test.cpp)
target_link_libraries(test testSet)

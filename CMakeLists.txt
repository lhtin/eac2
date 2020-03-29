cmake_minimum_required(VERSION 3.15)
project(EaC2)

set(CMAKE_CXX_STANDARD 17)

## 头文件位置
include_directories(utils src spec lex)

## lex库
add_library(lex lex/RE.cpp lex/FA.cpp lex/NFA.cpp lex/DFA.cpp lex/minDFA.cpp lex/WrapFA.cpp)
link_libraries(lex)

## utils库
add_library(utils utils/utils.cpp)
link_libraries(utils)

## test
add_executable(test test/test.cpp)
add_library(testSet test/test-re.cpp test/test-pl0.cpp)
target_link_libraries(test testSet)

add_executable(main main.cpp)
cmake_minimum_required(VERSION 3.15)
project(EaC2)

set(CMAKE_CXX_STANDARD 14)

add_executable(main main.cpp lex/RE.cpp lex/FA.cpp lex/RE2NFA.cpp lex/NFA2DFA.cpp lex/minDFA.cpp)
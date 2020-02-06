SRC = RE.cpp FA.cpp RE2NFA.cpp NFA2DFA.cpp minDFA.cpp lex.cpp
OBJ = $(SRC:%.cpp=%.o)

lex.a: ${SRC}
	clang++ -std=c++17 -c ${SRC}
	ar cr lex.a ${OBJ}

clean:
	rm -f *.o *.a
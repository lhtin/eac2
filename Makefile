.PHONY: clean run

run: main
	./main

main: main.cpp lex/lex.a utils/utils.a lex/lex.hpp utils/utils.hpp spec/lex-spec.hpp
	clang++ -std=c++17 main.cpp lex/lex.a utils/utils.a -o main

LEX_SRC = RE.cpp FA.cpp RE2NFA.cpp NFA2DFA.cpp minDFA.cpp lex.cpp
LEX_OBJS = ${LEX_SRC:%.cpp=%.o}
lex/lex.a: ${LEX_SRC:%.cpp=lex/%.cpp} lex/lex.hpp spec/lex-spec.hpp
	cd lex; \
	clang++ -std=c++17 -c ${LEX_SRC}; \
	ar cr lex.a ${LEX_OBJS}

UTILS_SRC = utils.cpp
UTILS_OBJS = ${UTILS_SRC:%.cpp=%.o}
utils/utils.a: ${UTILS_SRC:%.cpp=utils/%.cpp} utils/utils.hpp
	cd utils; \
	clang++ -std=c++17 -c ${UTILS_SRC}; \
	ar cr utils.a ${UTILS_OBJS}

clean:
	rm -f main */*.o */*.a
.PHONY: clean run

LEX_SRC = lex.cpp RE.cpp FA.cpp RE2NFA.cpp NFA2DFA.cpp minDFA.cpp
LEX_OBJS = ${LEX_SRC:%.cpp=lex/%.o}

UTILS_SRC = utils.cpp
UTILS_OBJS = ${UTILS_SRC:%.cpp=utils/%.o}

run: main
	./main

${LEX_OBJS}: %.o : %.cpp lex/lex.hpp spec/lex-spec.hpp
	clang++ -std=c++17 -c $< -o $@

${UTILS_OBJS}: %.o : %.cpp utils/utils.hpp
	clang++ -std=c++17 -c $< -o $@

main: main.cpp ${LEX_OBJS} ${UTILS_OBJS} lex/lex.hpp utils/utils.hpp spec/lex-spec.hpp
	clang++ -std=c++17 main.cpp ${LEX_OBJS} ${UTILS_OBJS} -o main -v

clean:
	rm -f main */*.o */*.a
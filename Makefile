.PHONY: clean run

LEX_SRC = RE.cpp FA.cpp RE2NFA.cpp NFA2DFA.cpp minDFA.cpp WrapFA.cpp
LEX_OBJS = ${LEX_SRC:%.cpp=lex/%.o}

UTILS_SRC = utils.cpp
UTILS_OBJS = ${UTILS_SRC:%.cpp=utils/%.o}

run: main
	./main spec/test1.pl0

${LEX_OBJS}: %.o : %.cpp lex/all.hpp
	clang++ -std=c++17 -c $< -o $@

${UTILS_OBJS}: %.o : %.cpp utils/utils.hpp
	clang++ -std=c++17 -c $< -o $@

main: main.cpp ${LEX_OBJS} ${UTILS_OBJS} utils/utils.hpp lex/lex.hpp syntax/LR1.hpp spec/pl0.hpp
	clang++ -std=c++17 main.cpp ${LEX_OBJS} ${UTILS_OBJS} -o main

clean:
	rm -f main */*.o */*.a
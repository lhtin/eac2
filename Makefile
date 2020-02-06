run: main
	./main

main: main.o
	(cd lex && make)
	(cd utils && make)
	clang++ -std=c++17 main.o lex/lex.a utils/utils.a -o main

main.o: main.cpp
	clang++ -std=c++17 -c main.cpp -o main.o

clean:
	(cd lex && make clean)
	(cd utils && make clean)
	rm -f main.o main
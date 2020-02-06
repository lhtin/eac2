SRC = utils.cpp
OBJ = $(SRC:%.cpp=%.o)

utils.a: ${SRC}
	clang++ -std=c++17 -c ${SRC}
	ar cr utils.a ${OBJ}

clean:
	rm -f *.o *.a
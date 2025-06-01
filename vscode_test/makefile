head=-I/yourpath/include
library=-L/yourpath/source
all:test1
test1:test1.cpp /yourpath/source/libmybind.so 
	g++ -o test1 test1.cpp $(head) $(library) -lmybind -g -std=c++20
clean:
	rm -f test1

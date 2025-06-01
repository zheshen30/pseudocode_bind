head=-I/home/test1/include
library=-L/home/test1/source
all:test1
test1:test1.cpp /home/test1/source/libmybind.so 
	g++ -o test1 test1.cpp $(head) $(library) -lmybind -g -std=c++20
clean:
	rm -f test1

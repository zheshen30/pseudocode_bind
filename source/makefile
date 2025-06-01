head=-I/yourpath/include
all: libmybind.so
libmybind.so: /yourpath/include/src_t.h mybind.cpp
	g++ -fPIC -shared -o libmybind.so mybind.cpp $(head) -O2 -std=c++20
clean:
	rm -f libmybind.so
	

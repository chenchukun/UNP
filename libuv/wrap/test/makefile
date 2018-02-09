CXXFLAGS=-std=c++11 -g ../libuv_cpp.a
LDFLAGS=-luv

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: main

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

main: main.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o main

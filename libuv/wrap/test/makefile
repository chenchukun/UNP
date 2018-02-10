CXXFLAGS=-std=c++11 -g
LDFLAGS=-luv ../libuv_cpp.a

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

CXXFLAGS=-std=c++11 -g
LDFLAGS=-luv ../libuv_cpp.a -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: testLoop

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

testLoop: testLoop.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o testLoop

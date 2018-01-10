CXXFLAGS=-std=c++11 -g
LDFLAGS=-luv

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: uvcat

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

uvcat: uvcat.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o uvcat

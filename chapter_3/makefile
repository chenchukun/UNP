CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: sockaddr

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

sockaddr: sockaddr.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o sockaddr

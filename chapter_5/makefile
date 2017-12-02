CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: echo

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

echo: echo.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o echo

CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a ../com/libnet.a -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: echo io

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

echo: echo.o
	g++ $^ -o $@ $(FLAGS)

io: io.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o echo io

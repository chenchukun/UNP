CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a ../com/libnet.a -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: select echo

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

select: select.o
	g++ $^ -o $@ $(FLAGS)

echo: echo.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o select echo

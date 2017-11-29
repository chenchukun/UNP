CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: socket connect bind listen accept

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

socket: socket.o
	g++ $^ -o $@ $(FLAGS)

connect: connect.o
	g++ $^ -o $@ $(FLAGS)

bind: bind.o
	g++ $^ -o $@ $(FLAGS)

listen: listen.o
	g++ $^ -o $@ $(FLAGS)

accept: accept.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o socket connect bind listen accept

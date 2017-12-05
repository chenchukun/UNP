CC=g++

CXXFLAGS=-std=c++11 -g
LDFLAGS=-lprotobuf -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: libutility.a libnet.a

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

libutility.a: utility.o
	ar -rc $@ $^

libnet.a: sock.o
	ar -rc $@ $^

clean:
	rm -rf *.o *.a

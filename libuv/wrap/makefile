CC=g++

CXXFLAGS=-std=c++11 -g
LDFLAGS= -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: libuv_cpp.a

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

libuv_cpp.a: TcpServer.o TcpConnection.o Buffer.o EventLoop.o
	ar -rc $@ $^

clean:
	rm -rf *.o *.a

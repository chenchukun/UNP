CXXFLAGS=-std=c++11 -g ../../com/libutility.a ../../com/libnet.a -I../../com
LDFLAGS=-luv

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: idle work dns thread process ThreadPool

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

idle: idle.o
	g++ $^ -o $@ $(FLAGS)

work: work.o
	g++ $^ -o $@ $(FLAGS)

dns: dns.o
	g++ $^ -o $@ $(FLAGS)

thread: thread.o
	g++ $^ -o $@ $(FLAGS)

process: process.o
	g++ $^ -o $@ $(FLAGS)

ThreadPool: ThreadPool.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o idle work dns thread process ThreadPool

CXXFLAGS=-std=c++11 -g ../../com/libutility.a ../../com/libnet.a -I../../com
LDFLAGS=-luv

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: handle timer poll signal

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

handle: handle.o
	g++ $^ -o $@ $(FLAGS)

timer: timer.o
	g++ $^ -o $@ $(FLAGS)

poll: poll.o
	g++ $^ -o $@ $(FLAGS)

signal: signal.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o handle timer poll signal

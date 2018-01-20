CXXFLAGS=-std=c++11 -g ../../com/libutility.a ../../com/libnet.a -I../../com
LDFLAGS=-luv

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: event_loop

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

event_loop: event_loop.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o event_loop

CXXFLAGS=-std=c++11 -g
LDFLAGS=-luv

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: event_loop idle

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

event_loop: event_loop.o
	g++ $^ -o $@ $(FLAGS)

idle: idle.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o event_loop idle
CXXFLAGS=-std=c++11 -g ../../com/libutility.a ../../com/libnet.a -I../../com
LDFLAGS=-luv

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: request async

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

request: request.o
	g++ $^ -o $@ $(FLAGS)

async: async.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o request async

CXXFLAGS=-std=c++11 -g
LDFLAGS=-lmuduo_net -lmuduo_base -lpthread -lmuduo_http

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: server

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

server: server.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o server

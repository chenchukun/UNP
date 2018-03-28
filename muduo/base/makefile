CXXFLAGS=-std=c++11 -g
LDFLAGS=-lmuduo_net -lmuduo_base -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: eventLoop

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

eventLoop: eventLoop.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o eventLoop

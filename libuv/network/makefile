CXXFLAGS=-std=c++11 -g
LDFLAGS=-luv

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: echoSrv tcp stream

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

tcp: tcp.o
	g++ $^ -o $@ $(FLAGS)

echoSrv: echoSrv.o
	g++ $^ -o $@ $(FLAGS)

stream: stream.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o echoSrv tcp stream

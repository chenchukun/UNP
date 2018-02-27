CXXFLAGS=-std=c++11 -g
LDFLAGS=-lmuduo_net -lmuduo_base -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: heartbeat

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

heartbeat: heartbeat.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o heartbeat

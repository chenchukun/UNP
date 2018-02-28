CXXFLAGS=-std=c++11 -g
LDFLAGS=-lmuduo_net -lmuduo_base -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: heartbeatSrv heartbeatCli

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

heartbeatSrv: heartbeatSrv.o
	g++ $^ -o $@ $(FLAGS)

heartbeatCli: heartbeatCli.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o heartbeatSrv heartbeatCli

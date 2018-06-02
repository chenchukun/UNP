CXXFLAGS=-std=c++11 -O2
LDFLAGS=-lmuduo_net -lmuduo_base -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: echoSrv

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

echoSrv: echoSrv.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o echoSrv

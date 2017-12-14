CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a ../com/libnet.a -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: sockopt buf_opt reuse nodelay

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

sockopt: sockopt.o
	g++ $^ -o $@ $(FLAGS)

buf_opt: buf_opt.o
	g++ $^ -o $@ $(FLAGS)

reuse: reuse.o
	g++ $^ -o $@ $(FLAGS)

nodelay: nodelay.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o sockopt buf_opt reuse nodelay

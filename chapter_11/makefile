CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a ../com/libnet.a -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: gethostbyname gethostbyaddr

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

gethostbyname: gethostbyname.o
	g++ $^ -o $@ $(FLAGS)

gethostbyaddr: gethostbyaddr.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o gethostbyname gethostbyaddr

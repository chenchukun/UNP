CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a ../com/libnet.a -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: testBuff echoSrv testConnect

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

testBuff: testBuff.o
	g++ $^ -o $@ $(FLAGS)

echoSrv: echoSrv.o
	g++ $^ -o $@ $(FLAGS)

testConnect: testConnect.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o testBuff echoSrv testConnect

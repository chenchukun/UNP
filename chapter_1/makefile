CXXFLAGS=-std=c++11 -g -I../com
LDFLAGS=../com/libutility.a ../com/libnet.a

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: daytimetcpcli daytimetcpsrv

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

daytimetcpcli: daytimetcpsrv.o
	g++ $^ -o $@ $(FLAGS)

daytimetcpsrv: daytimetcpsrv.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o daytimetcpcli daytimetcpsrv

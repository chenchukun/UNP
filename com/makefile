TARGET=libutility

CXXFLAGS=-std=c++11 -g
LDFLAGS=-lprotobuf -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: $(TARGET).a

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

$(TARGET).a: $(OBJECTS)
	ar -rc $@ $^

clean:
	rm -rf *.o *.a

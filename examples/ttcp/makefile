TARGET=ttcp

CXXFLAGS=-std=c++11 -g
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: $(TARGET)

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o ttcp

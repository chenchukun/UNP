TARGET=person

COMFLAG=-std=c++11
LDFLAGS=-lprotobuf -lpthread

FLAGS= $(COMFLAG) $(LDFLAGS)

all: $(TARGET)

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(COMFLAG) -c $< -o $@

$(TARGET): $(OBJECTS)
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o person 

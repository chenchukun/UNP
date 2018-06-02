TARGET=person

CXXFLAGS=-std=c++11 
LDFLAGS=-lprotobuf -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: $(TARGET)

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	g++ $^ -o $@ $(FLAGS)

protobuf: person.proto
	protoc --cpp_out=. person.proto
	mv person.pb.cc person.pb.cpp

clean:
	rm -rf *.o person 

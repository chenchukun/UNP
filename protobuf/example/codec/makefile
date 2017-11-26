TARGET=codecTest

CXXFLAGS=-std=c++11 -g
LDFLAGS=-lprotobuf -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: $(TARGET)

SOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	g++ $^ -o $@ $(FLAGS)

protobuf: search.proto
	protoc --cpp_out=. search.proto
	mv search.pb.cc search.pb.cpp

clean:
	rm -rf *.o codecTest

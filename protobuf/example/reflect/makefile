TARGET=reflect

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

protobuf: SearchRequest.proto SearchResponse.proto
	protoc --cpp_out=. $^
	mv SearchRequest.pb.cc SearchRequest.pb.cpp
	mv SearchResponse.pb.cc SearchResponse.pb.cpp

clean:
	rm -rf *.o reflect


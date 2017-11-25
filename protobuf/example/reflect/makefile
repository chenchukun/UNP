TARGET=reflect

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

protobuf: SearchRequest.proto SearchResponse.proto
	protoc --cpp_out=. $^
	mv SearchRequest.pb.cc SearchRequest.pb.cpp
	mv SearchResponse.pb.cc SearchResponse.pb.cpp

clean:
	rm -rf *.o reflect

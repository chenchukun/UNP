CXXFLAGS=-std=c++11 -g -DNDEBUG
LDFLAGS=-lgflags -lglog -lpthread

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: gflags_example glog_example

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

gflags_example: gflags_example.o gflags_declare.o
	g++ $^ -o $@ $(FLAGS)

glog_example: glog_example.o
	g++ $^ -o $@ $(FLAGS)

gflags_declare.o: gflags_declare.cpp
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o glog_example gflags_example

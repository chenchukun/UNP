CXXFLAGS=-std=c++11 -g -DNDEBUG
LDFLAGS=

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: spdlog_example fmt_example

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

spdlog_example: spdlog_example.o
	g++ $^ -o $@ $(FLAGS)

fmt_example: fmt_example.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o spdlog_example fmt_example

CXXFLAGS=-std=c++11 -g -DNDEBUG
LDFLAGS=-lhiredis_vip

FLAGS= $(CXXFLAGS) $(LDFLAGS)

all: redis_api

$(OBJECTS): %.o:%.cpp
	g++ $(CXXFLAGS) -c $< -o $@

redis_api: redis_api.o
	g++ $^ -o $@ $(FLAGS)

clean:
	rm -rf *.o redis_api

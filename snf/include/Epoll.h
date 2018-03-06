#ifdef USE_SYS_EPOLL
#ifndef EPOLL_H
#define EPOLL_H
#include "Poller.h"
#include <assert.h>
#include <vector>
#include <map>

namespace snf
{

class Epoll;

class EpollIterator : public PollerIterator
{
	friend Epoll;
public:
	int handle() {
		return epollEvent==NULL?-1:epollEvent->data.fd;
	}

	int event() {
		return epollEvent==NULL?-1:epollEvent->events;
	}

private:
	struct epoll_event *epollEvent;
};

class Epoll : public Poller
{
	friend EpollIterator;
public:
	using EpollEvent = struct epoll_event;
	enum {EPOLL_MAX=1024};

	Epoll() : currIndex(0), activeNum(0) {
		epfd = epoll_create(EPOLL_MAX);
		assert(epfd >= 0);
//		epollfds.reserve(128);
		epollfds.resize(128);
	}

	~Epoll() {}

	int poll(double sec =-1);

	int addHandle(int handle, int event);

	int delHandle(int handle);

	int setEvent(int handle, int event);

	int getEvent(int handle);

	EpollIterator* getActiveIterator();

	EpollIterator* getIterator(int index);

	int size() {
		return listenHandleMap.size();
	}

private:
	int epfd;

	int currIndex;

	int activeNum;

	std::vector<EpollEvent> epollfds;

	std::map<int, EpollEvent> listenHandleMap;

	EpollIterator iterator;
	
};

}

#endif
#endif

#ifndef USE_SYS_EPOLL
#ifndef POLL_H
#define POLL_H
#include "Poller.h"
#include <stddef.h>
#include <poll.h>
#include <vector>

namespace snf
{

class Poll;

class PollIterator : public PollerIterator
{
	friend Poll;
public:
	PollIterator(): it(NULL) {}

	int handle() {
		return it==NULL?-1:it->fd;
	}

	int event() {
		return it==NULL?-1:it->revents;
	}
private:
	struct pollfd *it;
};

class Poll : public Poller
{
	friend PollIterator;
public:
	using Pollfd = struct pollfd;

	Poll() : handleIndexVec(1024, -1), activeNum(0), currIndex(0) {
		pollfds.reserve(128);
	}

	~Poll() {}

	int poll(double sec=-1);

	int addHandle(int handle, int event);

	int delHandle(int handle);

	int setEvent(int handle, int event);

	int getEvent(int handle);

	PollIterator* getActiveIterator();

	PollIterator* getIterator(int index);

	int size() {
		return pollfds.size();
	}

private:
	std::vector<Pollfd> pollfds;

	std::vector<int> handleIndexVec;

	int activeNum;

	int currIndex;

	PollIterator iterator;

};

}

#endif
#endif

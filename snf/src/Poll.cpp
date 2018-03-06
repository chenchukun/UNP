#ifndef USE_SYS_EPOLL
#include "Poll.h"
#include <assert.h>
#include <errno.h>

namespace snf
{

int Poll::addHandle(int handle, int event) 
{
	// handle 已经存在
	if (handle < handleIndexVec.size() && handleIndexVec[handle] >=0) {
		errno = EEXIST;
		return -1;
	}
//	pollfds.push_back(Pollfd{handle, reinterpret_cast<short>(event, 0});
	pollfds.push_back(Pollfd{handle, static_cast<short>(event, 0});  // 为什么不能是reinterpret_cast
	int index = pollfds.size() - 1;
	assert(index >= 0);
	if (handle >= handleIndexVec.size()) {
		handleIndexVec.reserve(handle*2);
		handleIndexVec.insert(handleIndexVec.end(), handle*2-index-1);
	}
	handleIndexVec[handle] = index;
	// update
	return 0;
}

int Poll::delHandle(int handle) 
{
	// handle 不存在
	if (handle >= handleIndexVec.size() || handleIndexVec[handle] < 0) {
		errno = ENOENT;
		return -1;
	}
	int index = handleIndexVec[handle];
	assert(index >= 0);
	auto it = pollfds.begin() + index;
	pollfds.erase(it);
	handleIndexVec[handle] = -1;
	for (int i=index; i<pollfds.size(); ++i) {
		--handleIndexVec[pollfds[i].fd];
	}
	// update
	return 0;
}

int Poll::setEvent(int handle, int event) 
{
	// handle 不存在
	if (handle >= handleIndexVec.size() || handleIndexVec[handle] < 0) {
		errno = ENOENT;
		return -1;
	}
	int index = handleIndexVec[handle];
	assert(index >= 0);
//	pollfds[index].events = reinterpret_cast<short>(event);
	pollfds[index].events = static_cast<short>(event);  // 为什么不能是reinterpret_cast
	// update
	return 0;
}

int Poll::getEvent(int handle) 
{
	// handle 不存在
	if (handle >= handleIndexVec.size() || handleIndexVec[handle] < 0) {
		errno = ENOENT;
		return -1;
	}
	int index = handleIndexVec[handle];
	assert(index >= 0);
	return pollfds[index].events;
}

PollIterator* Poll::getActiveIterator()
{
	iterator.it = NULL;
	if (activeNum <= 0) {
		return NULL;
	}
	for (int i=currIndex; i < pollfds.size(); ++i) {
		if (pollfds[i].revents != 0) {
			iterator.it = &pollfds[i];
			currIndex = i+1;
			--activeNum;
			return &iterator;
		}
	}
	return NULL;
}

PollIterator* Poll::getIterator(int index)
{
	if (index >= pollfds.size()) {
		return NULL;
	}
	iterator.it = &pollfds[index];
	return &iterator;
}

int Poll::poll(double sec) 
{
	activeNum = 0;
	currIndex = 0;
	int msec = sec==-1?-1:static_cast<int>(sec*1000);
	int ret;
	while (true) {
		ret = ::poll(&pollfds[0], pollfds.size(), msec);
		if (ret == -1) {
			if (errno == EINTR) {
				continue;
			}
			return -1;
		}
		break;
	}
	activeNum = ret;
	return ret;
}

}
#endif

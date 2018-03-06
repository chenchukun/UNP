#ifdef USE_SYS_EPOLL
#include "Epoll.h"
#include <errno.h>

namespace snf
{
/*
	epoll_ctl(int epfd, int op, int fd, struct epoll_event *ev);
	EPOLL_CTL_ADD：添加fd到epfd的兴趣列表中，若已存在将出现EEXIST错误。
	EPOLL_CTL_MOD：修改文件描述符fd上设定的事件，事件信息由ev指定，若不存在将出现ENOENT错误。
	EPOLL_CTL_DEL：将fd从epfd的兴趣列表中移除，若不存在将出现ENOENT错误。
	int epoll_wait(int epfd, struct epoll_event *evlist, int maxevents, int timeout);
*/

int Epoll::poll(double sec)
{
	int msec = sec==-1?-1:static_cast<int>(sec*1000);
	currIndex = 0;
	activeNum = 0;
	int ret;
	while (true) {
		ret = ::epoll_wait(epfd, &epollfds[0], epollfds.size(), msec);
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

int Epoll::addHandle(int handle, int event)
{
	if (listenHandleMap.count(handle) > 0) {
		errno = EEXIST;
		return -1;
	}
	EpollEvent epollEvent;
	epollEvent.events = event;
	epollEvent.data.fd = handle;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, handle, &epollEvent);
	if (ret != -1) {
		listenHandleMap[handle] = epollEvent;
		if (size() >= epollfds.size()) {
			epollfds.resize(size()*2);
		}
	}
	return ret;
}

int Epoll::delHandle(int handle)
{
	if (listenHandleMap.count(handle) == 0) {
		errno = ENOENT;
		return -1;
	}
	EpollEvent epollEvent;
	epollEvent.data.fd = handle;
	int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, handle, &epollEvent);
	if (ret != -1) {
		listenHandleMap.erase(handle);
	}
	return ret;
}

int Epoll::setEvent(int handle, int event)
{
	if (listenHandleMap.count(handle) == 0) {
		errno = ENOENT;
		return -1;
	}
	EpollEvent epollEvent = listenHandleMap[handle];
	epollEvent.events = event;
	int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, handle, &epollEvent);
	if (ret != -1) {
		listenHandleMap[handle] = epollEvent;
	}
	return ret;
}

int Epoll::getEvent(int handle)
{
	if (listenHandleMap.count(handle) == 0) {
		errno = ENOENT;
		return -1;
	}
	return listenHandleMap[handle].events;
}

EpollIterator* Epoll::getActiveIterator()
{
	if (activeNum <= 0) {
		return NULL;
	}
	iterator.epollEvent = &epollfds[currIndex++];
	--activeNum;
	return &iterator;
}

EpollIterator* Epoll::getIterator(int index)
{
	return NULL;
}

}

#endif

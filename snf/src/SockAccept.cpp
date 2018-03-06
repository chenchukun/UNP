#include "SockAccept.h"
#include "Poller.h"
#include <iostream>

namespace snf
{


int SockAccept::open(const SockAddr &sockAddr, int backlog, int proto)
{
	int ret = SockHandle::open(sockAddr.getFamily(), SOCK_STREAM, proto);
	if (ret == -1) {
		return -1;
	}
	setUseAddr(true);
	ret = ::bind(getHandle(), sockAddr.getAddr(), sockAddr.getAddrLength());
	if (ret == -1) {
		return -1;
	}
	ret = ::listen(getHandle(), backlog);
	return ret;
}

int SockAccept::accept(SockStream &sockStream)
{
	int sock = ::accept(getHandle(), NULL, NULL);
	if (sock < 0) {
		return -1;
	}
	sockStream.setHandle(sock);
	return 0;
}

int SockAccept::accept(SockStream &sockStream, double timeout)
{
	bool block;
	int ret = getNonBlock(block);
	if (ret != 0) {
		return -1;
	}
	ret = setNonBlock(true);
	if (ret != 0) {
		return -1;
	}
	auto poller = Poller::create();
	poller->addHandle(getHandle(), SNF_POLLIN);
	ret = poller->poll(timeout);
	if (ret == 0) {
		setNonBlock(block);
		errno = ETIMEDOUT;
		return -1;
	}
	if (ret == -1) {
		setNonBlock(block);
		return -1;
	}
	int sock = ::accept(getHandle(), NULL, NULL);
	if(sock < 0) {
		setNonBlock(block);
		return -1;
	}
	sockStream.setHandle(sock);
	setNonBlock(block);
	sockStream.setNonBlock(block);
	return 0;
}

}

#include "SockConnect.h"
#include "Poller.h"
#include <sys/select.h>
#include <errno.h>
#include <time.h>

namespace snf
{

int SockConnect::connect(SockStream &sockStream, const SockAddr &remoteAddr, 
		double timeout, const SockAddr *localAddr, int proto)
{
	int ret = this->connect(sockStream, remoteAddr, localAddr, proto, true);
	if (ret == -1) {
		sockStream.close();
		return -1;
	}
	if (ret == EINPROGRESS) {
		auto poller = Poller::create();
		poller->addHandle(sockStream.getHandle(), SNF_POLLIN|SNF_POLLOUT);
		ret = poller->poll(timeout);
		if (ret == 0) {
			errno = ETIMEDOUT;
			sockStream.close();
			return -1;
		}
		if (ret == -1) {
			sockStream.close();
			return -1;
		}
		Poller::Iterator activeIt = poller->getActiveIterator();
		if (activeIt == NULL) {
			sockStream.close();
			return -1;
		}
		int event = activeIt->event();
		if ((event&SNF_POLLIN) || (event&SNF_POLLOUT)) {
			int error;
			ret = sockStream.getError(error);
			if (ret == -1) {
				sockStream.close();
				return -1;
			}
			if (error) {
				errno = error;
				sockStream.close();
				return -1;
			}
		}
		else {
			return -1;
		}
	}
//	sockStream.setNonBlock(false);
	return 0;
}

int SockConnect::connect(SockStream &sockStream, const SockAddr &remoteAddr, const SockAddr *localAddr, int proto, bool nonblock)
{
	int ret = sockStream.open(remoteAddr.getFamily(), SOCK_STREAM, proto);
	if (ret != 0) {
		return ret;
	}

	if (localAddr != NULL) {
		ret = ::bind(sockStream.getHandle(), localAddr->getAddr(), localAddr->getAddrLength());
		if (ret != 0) {
			return ret;
		}
	}

	sockStream.setNonBlock(nonblock);
	ret = ::connect(sockStream.getHandle(), remoteAddr.getAddr(), remoteAddr.getAddrLength());
	if (ret == 0) {
		return 0;
	}
	if (ret == -1 && errno == EINPROGRESS) {
		return EINPROGRESS;
	}
	return -1;
}

}

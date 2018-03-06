#ifndef SOCK_HANDLE_H
#define SOCK_HANDLE_H
#include "IoHandle.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

namespace snf
{

class SockAddr;

class SockHandle : public IoHandle
{
public:
	SockHandle(int fd): IoHandle(fd) {}

	SockHandle(): IoHandle(-1) {}

	~SockHandle() {}

	int open(int family, int type, int proto = 0);

	int getLocalAddr(SockAddr &localAddr) const;

	int getPeerAddr(SockAddr &peerAddr) const;

	template<typename T>
	int setOption(int level, int optname, const T &optval) const;

	template<typename T>
	int getOption(int level, int optname, T &optval) const;

	inline int getError(int &error) const;

	inline int getRcvBuf(int &bufSize) const;

	inline int setRcvBuf(int bufSize) const;

	inline int getSndBuf(int &bufSize) const;

	inline int setSndBuf(int bufSize) const;

	inline int getRcvLowat(int &lowat) const;

	inline int setRcvLowat(int lowat) const;

	inline int getSndLowat(int &lowat) const;

	inline int setSndLowat(int lowat) const;

	inline int setUseAddr(bool use) const;

	inline int setUsePort(bool use) const;

	inline int setNodelay(bool no) const;

	inline int shutdownWrite() const;
	
	inline int shutdownRead() const;

	inline int shutdown() const;

private:
};

template<typename T>
int SockHandle::setOption(int level, int optname, const T &optval) const
{
	if ( !valid() ) {
		return -1;
	}
	return setsockopt(getHandle(), level, optname, &optval, sizeof(optval));
}

template<typename T>
int SockHandle::getOption(int level, int optname, T &optval) const
{
	if ( !valid() ) {
		return -1;
	}
	socklen_t len = sizeof(optval);
	return getsockopt(getHandle(), level, optname, &optval, &len);
}

int SockHandle::getError(int &error) const {
	return getOption(SOL_SOCKET, SO_ERROR, error);
}

int SockHandle::getRcvBuf(int &bufSize) const {
	return getOption(SOL_SOCKET, SO_RCVBUF, bufSize);
}

int SockHandle::setRcvBuf(int bufSize) const {
	return setOption(SOL_SOCKET, SO_RCVBUF, bufSize);
}

int SockHandle::getSndBuf(int &bufSize) const {
	return getOption(SOL_SOCKET, SO_SNDBUF, bufSize);
}

int SockHandle::setSndBuf(int bufSize) const {
	return setOption(SOL_SOCKET, SO_SNDBUF, bufSize);
}

int SockHandle::getRcvLowat(int &lowat) const {
	return getOption(SOL_SOCKET, SO_RCVLOWAT, lowat);
}

int SockHandle::setRcvLowat(int lowat) const {
	return setOption(SOL_SOCKET, SO_RCVLOWAT, lowat);
}

int SockHandle::getSndLowat(int &lowat) const {
	return getOption(SOL_SOCKET, SO_SNDLOWAT, lowat);
}

int SockHandle::setSndLowat(int lowat) const {
	return setOption(SOL_SOCKET, SO_SNDLOWAT, lowat);
}

int SockHandle::setUseAddr(bool use) const {
	int u = use?1:0;
	return setOption(SOL_SOCKET, SO_REUSEADDR, u);
}

int SockHandle::setUsePort(bool use) const {
	int u = use?1:0;
	return setOption(SOL_SOCKET, SO_REUSEPORT, u);
}

int SockHandle::setNodelay(bool no) const {
	int n = no?1:0;
	return setOption(IPPROTO_TCP, TCP_NODELAY, n);
}

int SockHandle::shutdownWrite() const
{
	return ::shutdown(getHandle(), SHUT_WR);
}

int SockHandle::shutdown() const
{
	return ::shutdown(getHandle(), SHUT_RDWR);
}

int SockHandle::shutdownRead() const
{
	return ::shutdown(getHandle(), SHUT_RD);
}

}

#endif

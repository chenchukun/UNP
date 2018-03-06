#include "SockHandle.h"
#include "SockAddr.h"
#include <sys/socket.h>

namespace snf
{

int SockHandle::open(int family, int type, int proto) 
{
	int sock = socket(family, type, proto);
	if (sock < 0) {
		return -1;
	}
	setHandle(sock);
	return 0;
}

int SockHandle::getLocalAddr(SockAddr &localAddr) const 
{
	if ( !valid() ) {
		return -1;
	}
	socklen_t len = localAddr.getAddrLength();
	return getsockname(getHandle(), localAddr.getAddr(), &len);
}

int SockHandle::getPeerAddr(SockAddr &remoteAddr) const
{
	if ( !valid() ) {
		return -1;
	}
	socklen_t len = remoteAddr.getAddrLength();
	return getpeername(getHandle(), remoteAddr.getAddr(), &len);
}

}

#ifndef SOCK_CONNECT_H
#define SOCK_CONNECT_H
#include "SockStream.h"
#include "SockAddr.h"

namespace snf
{

class SockConnect
{
public:
	SockConnect() {}

	~SockConnect() {}

	int connect(SockStream &sockStream, const SockAddr &remoteAddr, 
			double timeout, const SockAddr *localAddr=NULL, int proto=0);

	int connect(SockStream &sockStream, const SockAddr &remoteAddr, const SockAddr *localAddr=NULL, int proto=0, bool nonblock=false);

private:
};

}

#endif

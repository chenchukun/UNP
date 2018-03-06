#ifndef SOCK_ACCEPT_H
#define SOCK_ACCEPT_H
#include "SockStream.h"
#include "SockAddr.h"
#include "SockHandle.h"

namespace snf
{

class SockAccept : public SockHandle
{
public:
	int open(const SockAddr &sockAddr, int backlog=100, int proto=0);

	int accept(SockStream &sockStream);

	int accept(SockStream &sockStream, double timeout);

private:
};

}

#endif

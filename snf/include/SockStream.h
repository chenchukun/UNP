#ifndef SOCK_STREAM_H
#define SOCK_STREAM_H
#include "SockHandle.h"
#include <sys/uio.h>
#include <unistd.h>

namespace snf
{

class SockStream : public SockHandle
{
public:

	inline int read(void *buff, int len) const;

	inline int write(const void *buff, int len) const;

	inline int readv(struct iovec *iov, int iovCnt) const;

	inline int writev(const iovec *iov, int iovCnt) const;

private:
};

int SockStream::read(void *buff, int len) const
{
	return ::read(getHandle(), buff, len);
}

int SockStream::write(const void *buff, int len) const
{
	return ::write(getHandle(), buff, len);
}

int SockStream::readv(struct iovec *iov, int iovCnt) const
{
	return ::readv(getHandle(), iov, iovCnt);
}

int SockStream::writev(const struct iovec *iov, int iovCnt) const
{
	return ::writev(getHandle(), iov, iovCnt);
}

}

#endif

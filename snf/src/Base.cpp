#include "Base.h"
#include <errno.h>
#include <unistd.h>

namespace snf 
{

ssize_t readn(int fd, void *buff, size_t len) 
{
	size_t nleft = len;
	ssize_t nread;
	char *ptr = static_cast<char*>(buff);

	while (nleft > 0) {
		nread = read(fd, ptr, nleft);
		if (nread < 0) {
			if (errno == EINTR) {
				nread = 0;
			}
			else {
				return -1;
			}
		}
		else if (nread == 0) {
			break;
		}
		nleft -= nread;
		ptr += nread;
	}
	return len - nleft;
}

ssize_t writen(int fd, const void *buff, size_t len)
{
	size_t nleft = len;
	ssize_t nwrite;
	const char *ptr = static_cast<const char*>(buff);
	while (nleft > 0) {
		nwrite = write(fd, ptr, nleft);
		if (nwrite <= 0) {
			if (nwrite < 0 && errno == EINTR) {
				nwrite = 0;
			}
			else {
				return -1;
			}
		}
		nleft -= nwrite;
		ptr += nwrite;
	}
	return len;
}

ssize_t readline(int fd, void *buff, size_t maxlen)
{
	ssize_t n, nread;
	char ch;
	char *ptr = static_cast<char *>(buff);
	for (n=1; n<maxlen; ++n) {
		nread = read(fd, &ch, 1);
		if (nread == 1) {
			*ptr++ = ch;
			if (ch == '\n') {
				break;
			}
		}
		else if (nread == 0) {
			*ptr = 0;
			return n-1;
		}
		else {
			if (errno == EINTR) {
				--n;
				continue;
			}
			return -1;
		}
	}
	*ptr = 0;
	return n;
}

}

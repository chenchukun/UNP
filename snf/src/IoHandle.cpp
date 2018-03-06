#include "IoHandle.h"
#include <unistd.h>

namespace snf
{

int IoHandle::setNonBlock(bool opt) const 
{
	if ( !valid() ) {
		return -1;
	}
	int flags = fcntl(handle, F_GETFL, 0);
	if (flags < 0) {
		return -1;
	}
	if (opt) {
		flags |= O_NONBLOCK;
	}
	else {
		flags &= ~O_NONBLOCK;
	}
	flags = fcntl(handle, F_SETFL, flags);
	if (flags < 0) {
		return -1;
	}
}

int IoHandle::getNonBlock(bool &opt) const 
{
	if ( !valid() ) {
		return -1;
	}
	int flags = fcntl(handle, F_GETFL, 0);
	if (flags < 0) {
		return -1;
	}
	opt = flags & O_NONBLOCK;
	return 0;
}

int IoHandle::getCloExec(bool &opt) const 
{
	if ( !valid() ) {
		return -1;
	}
	int flags = fcntl(handle, F_GETFD, 0);
	if (flags < 0) {
		return -1;
	}
	return flags & O_CLOEXEC;
	return 0;
}


int IoHandle::setCloExec(bool opt) const 
{
	if ( !valid() ) {
		return -1;
	}
	int flags = fcntl(handle, F_GETFD, 0);
	if (flags < 0) {
		return -1;
	}
	if (opt) {
		flags |= O_CLOEXEC;
	}
	else {
		flags &= ~O_CLOEXEC;
	}
	fcntl(handle, F_SETFD, flags);
	if (flags < 0) {
		return -1;
	}
}

int IoHandle::close()
{
	int ret = ::close(handle);
	if (ret == 0) {
		handle = -1;
	}
	return ret;
}

}

#ifndef IO_HANDLE_H
#define IO_HANDLE_H
#include <unistd.h>
#include <fcntl.h>

namespace snf
{

class IoHandle
{
public:
	IoHandle(int fd): handle(fd) {}

	IoHandle(): handle(-1) {}

	virtual ~IoHandle() {}

	void setHandle(int fd) {
		handle = fd;
	}

	int getHandle() const {
		return handle;
	}

	int setNonBlock(bool opt) const;

	int getNonBlock(bool &opt) const;

	int setCloExec(bool opt) const;

	int getCloExec(bool &opt) const;

	bool valid() const {
		return handle >= 0;
	}

	IoHandle dup() const {
		if ( !valid() ) {
			return IoHandle(-1);
		}
		int newFd = ::dup(handle);
		return IoHandle(newFd);
	}

	int close();

private:
	int handle;
};

}

#endif

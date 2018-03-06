#include "Pipe.h"
#include "Base.h"

namespace snf
{

int Pipe::setNonBlock(bool op)
{
	IoHandle ioHandle;
	ioHandle.setHandle(handle[0]);
	int ret = ioHandle.setNonBlock(op);
	if (ret == -1) {
		return -1;
	}
	ioHandle.setHandle(handle[1]);
	ret = ioHandle.setNonBlock(op);
	if (ret == -1) {
		return -1;
	}
	return 0;
}

int Pipe::clear() {
	if (size() == 0) {
		return 0;
	}
	char *buff = new char[size()];
	int len = readn(handle[0], buff, size());
	if (len != size()) {
		delete buff;
		return -1;
	}
	delete buff;
	return 0;
}

int Pipe::read(void *buff, int n)
{
	int len = ::read(handle[0], buff, n);
	if (len > 0) {
		size_ -= len;
	}
	return len;
}

int Pipe::write(const void *buff, int n)
{
	int len = ::write(handle[1], buff, n);
	if (len > 0) {
		size_ += len;
	}
	return len;
}

}

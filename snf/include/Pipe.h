#ifndef PIPE_H
#define PIPE_H
#include "IoHandle.h"
#include <unistd.h>
#include <assert.h>
#include <atomic>

namespace snf
{

class Pipe
{
public:
	inline Pipe();

	inline ~Pipe();

	int setNonBlock(bool op);

	int read(void *buff, int n);

	int write(const void *buff, int n);

	int getReadHandle() {
		return handle[0];
	}

	int getWriteHandle() {
		return handle[1];
	}

	int size() {
		return size_;
	}

	bool empty() {
		return size() == 0;
	}

	int clear();

private:
	int handle[2];

	std::atomic_int size_;
};

Pipe::Pipe() : size_(0)
{
	int ret = pipe(handle);	
	assert(ret == 0);
}

Pipe::~Pipe() {
	close(handle[0]);
	close(handle[1]);
}

}

#endif

#include "CountDownLatch.h"

namespace snf
{

void CountDownLatch::countDown()
{
	MutexLockGuard lockGuard(mutex);
	--count;
	if (count == 0) {
		cond.notifyAll();
	}
}

void CountDownLatch::wait() 
{
	MutexLockGuard lockGuard(mutex);
	while (count != 0) {
		cond.wait();
	}
}

}

#ifndef COUNT_DOWN_LATCH_H
#define COUNT_DOWN_LATCH_H
#include "Condition.h"

namespace snf
{

class CountDownLatch : public Uncopyable
{
public:
	explicit CountDownLatch(int c)
		: count(c),
		  cond(mutex){}

	void countDown();

	void wait();

	int getCount() const {
		MutexLockGuard lockGuard(mutex);
		return count;
	}

private:
	int count;
	
	mutable Mutex mutex;

	Condition cond;
	
};

}

#endif

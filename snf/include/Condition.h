#ifndef CONDITION_H
#define CONDITION_H
#include <pthread.h>
#include "Mutex.h"
#include "Uncopyable.h"

namespace snf
{

class Condition : public Uncopyable
{
public:
	Condition(Mutex &mutex)
		: mutex(mutex) {
		assert(pthread_cond_init(&cond, NULL)==0);
	}

	~Condition() {
		pthread_cond_destroy(&cond);
	}

	int notify() {
		return pthread_cond_signal(&cond);
	}

	int notifyAll() {
		return pthread_cond_broadcast(&cond);
	}

	int wait() {
		return pthread_cond_wait(&cond, &mutex.mutex);
	}

	int wait(double second);

private:
	pthread_cond_t cond;
	Mutex &mutex;	
};

}

#endif

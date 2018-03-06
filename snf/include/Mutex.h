#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>
#include "Uncopyable.h"
#include <assert.h>

namespace snf
{

class Condition;

class Mutex : public Uncopyable
{
	friend class Condition;
public:
	Mutex() {
		assert(pthread_mutex_init(&mutex, NULL)==0);
	}

	~Mutex() {
		pthread_mutex_destroy(&mutex);
	}

	int lock() {
		return pthread_mutex_lock(&mutex);
	}

	int trylock() {
		return pthread_mutex_trylock(&mutex);
	}

	int unlock() {
		return pthread_mutex_unlock(&mutex);
	}

private:
	pthread_mutex_t mutex;
};

class MutexLockGuard : public Uncopyable
{
public:
	MutexLockGuard(Mutex &mutex)
		: mutex(mutex) {
		mutex.lock();		
	}

	~MutexLockGuard() {
		mutex.unlock();
	}
private:
	Mutex &mutex;
};

}

#endif

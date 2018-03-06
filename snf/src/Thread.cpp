#include "Thread.h"

namespace snf
{

namespace CurrThread
{
	__thread pid_t currTid = 0;
}

Thread::~Thread() 
{
	if (_start && !_join && !_detach) {
		detach();
	}
}

void* Thread::threadRun(void *arg)
{
	ThreadData *threadData = static_cast<ThreadData*>(arg);
	auto ptid = threadData->ptid.lock();
	if (ptid) {
		*ptid = CurrThread::tid();
		ptid.reset();
	}
	auto palive = threadData->palive.lock();
	if (palive) {
		*palive = true;
	}
	threadData->threadFun();
	if (palive) {
		*palive = false;
		palive.reset();
	}
	delete threadData;
	return NULL;
}

void Thread::start()
{
	ThreadData *threadData = new ThreadData{threadFun, ptid, palive};
	assert(pthread_create(&thread, NULL, &Thread::threadRun, static_cast<void*>(threadData)) == 0);
	_start = true;
}

void Thread::join()
{
	assert(_start);
	assert(!_join);
	assert(!_detach);
	pthread_join(thread, NULL);
	_join = true;
}

void Thread::detach() 
{
	assert(_start);
	assert(!_join);
	assert(!_detach);
	pthread_detach(thread);
	_detach = true;
}

}

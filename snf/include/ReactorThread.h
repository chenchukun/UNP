#ifndef REACTOR_THREAD_H
#define REACTOR_THREAD_H
#include "Reactor.h"
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

namespace snf
{

class ReactorThread
{
public:
	ReactorThread();

	~ReactorThread() {}

	Reactor* start();

private:
	void reactorFunc();

	Reactor *_reactor;

	Thread reactorThread;

	Mutex mutex;

	Condition cond;
};

}

#endif

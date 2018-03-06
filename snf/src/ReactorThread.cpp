#include "ReactorThread.h"

namespace snf
{

ReactorThread::ReactorThread()
	: _reactor(NULL),
	  reactorThread(std::bind(&ReactorThread::reactorFunc, this)),
	  cond(mutex)
{
}

Reactor* ReactorThread::start()
{
	reactorThread.start();
	MutexLockGuard lock(mutex);
	while (_reactor == NULL) {
		cond.wait();
	}
	return _reactor;
}

void ReactorThread::reactorFunc()
{
	Reactor reactor;
	{
		MutexLockGuard lock(mutex);
		_reactor = &reactor;
		cond.notify();
	}
	reactor.run();
}

}

#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "BoundedBlockingQueue.h"
#include "Thread.h"
#include <vector>
#include <atomic>

namespace snf
{

class ThreadPool : public Uncopyable
{
public:
	explicit ThreadPool(uint32_t pn, uint32_t tn=BoundedBlockingQueue<Thread::ThreadFun>::MAX_SIZE)
		: running(false), 
		  busy(0), 
		  poolSize(pn), 
		  tasks(tn),
		  runNum(0) {}

	~ThreadPool();

	void run(const Thread::ThreadFun &threadFun);

	bool run(const Thread::ThreadFun &threadFun, double second);

	void start();

	void stop();

	uint32_t size() {
		return poolSize;
	}

	uint32_t getBusyCount() {
		return busy;
	}

private:
	void runThread();

	std::vector<Thread*> threads;

	BoundedBlockingQueue<Thread::ThreadFun> tasks;

	bool running;

	std::atomic_int busy;

	uint32_t poolSize;

	std::atomic_int runNum;

};

}

#endif

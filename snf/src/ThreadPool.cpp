#include "ThreadPool.h"
#include <iostream>

namespace snf
{

ThreadPool::~ThreadPool()
{
	for (int i=0; i<threads.size(); ++i) {
		delete threads[i];
	}
}

void ThreadPool::start()
{
	threads.reserve(poolSize);	
	running = true;
	// 如果存储的是不可拷贝的，reserve无效？不是不会构造吗？ 
	for (int i=0; i<poolSize; ++i) {
		auto thread = new Thread(std::bind(&ThreadPool::runThread, this));
		threads.push_back(thread);
		thread->start();
	}
	while (runNum < poolSize);
}

void ThreadPool::stop()
{
	assert(running);
	running = false;
	for (int i=0; i<threads.size(); ++i) {
		threads[i]->join();
	}
}

void ThreadPool::run(const Thread::ThreadFun &threadFun)
{
	tasks.put(threadFun);
}

bool ThreadPool::run(const Thread::ThreadFun &threadFun, double second)
{
	return tasks.offer(threadFun, second);
}

void ThreadPool::runThread()
{
	++runNum;
	while (running) {
		Thread::ThreadFun task = NULL;
		if (!tasks.poll(task, 1)) {
			continue;
		}
		if (task != NULL) {
			++busy;
			task();
			--busy;
		}
	}
}

}

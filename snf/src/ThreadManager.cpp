#include "ThreadManager.h"

namespace snf
{

ThreadManager::~ThreadManager()
{
	auto begin = threads.begin();
	while (begin != threads.end()) {
		delete *begin;
		++begin;
	}
}

void ThreadManager::create(Thread::ThreadFun threadFun, uint32_t n) 
{
	for (int i=0; i<n; ++i) {
		Thread *thread = new Thread(threadFun);	
		thread->start();
		threads.push_back(thread);
	}
}

void ThreadManager::waitAll()
{
	auto begin = threads.begin();
	while (begin != threads.end()) {
		(*begin)->join();
		++begin;
	}
}

}

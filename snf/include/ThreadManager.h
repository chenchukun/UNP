#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H
#include "Thread.h"
#include <vector>

namespace snf
{

class ThreadManager : public Uncopyable
{
public:
	ThreadManager() = default;

	~ThreadManager();

	void create(Thread::ThreadFun threadFun, uint32_t n=1);

	void waitAll();

	uint32_t getCount() {
		return threads.size();
	}

private:
	std::vector<Thread*> threads;
};

}

#endif

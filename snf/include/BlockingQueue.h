#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H
#include "Thread.h"
#include "Condition.h"
#include <queue>

namespace snf
{

template<typename T>
class BlockingQueue : public Uncopyable
{
public:
	BlockingQueue()
		: cond(mutex) {}

	void put(const T &elem);

//	bool offer(const T &elem);

//	bool offer(const T &elem, double second);

	T take();

	bool poll(T &elem, double second);

	uint32_t size() {
		MutexLockGuard lockGuard(mutex);
		return elemQue.size();
	}

	bool empty() {
		MutexLockGuard lockGuard(mutex);
		return elemQue.empty();
	}
	
private:
	std::queue<T> elemQue;

	Mutex mutex;

	Condition cond;
};

template<typename T>
void BlockingQueue<T>::put(const T &elem)
{
	{
		MutexLockGuard lockGuard(mutex);
		elemQue.push(elem);
	}
	cond.notify();
}

template<typename T>
T BlockingQueue<T>::take()
{
	MutexLockGuard lockGuard(mutex);
	while (elemQue.empty()) {
		cond.wait();
	}
	T elem = elemQue.front();
	elemQue.pop();
	return elem;
}

template<typename T>
bool BlockingQueue<T>::poll(T &elem, double second)
{
	MutexLockGuard lockGuard(mutex);
	while (elemQue.empty()) {
		int ret = cond.wait(second);
		if (ret == ETIMEDOUT) {
			return false;
		}
	}
	elem = elemQue.front();
	elemQue.pop();
	return true;
}

}

#endif

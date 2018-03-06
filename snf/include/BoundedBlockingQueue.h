#ifndef BOUNDED_BLOCKING_QUEUE_H
#define BOUNDED_BLOCKING_QUEUE_H
#include "Thread.h"
#include "Condition.h"
#include <queue>

namespace snf
{

template<typename T>
class BoundedBlockingQueue : public Uncopyable
{
public:
	enum{MAX_SIZE=4294967295};

	explicit BoundedBlockingQueue(uint32_t n)
		: noEmptyCond(mutex), noFullCond(mutex), maxSize(n) {
		assert(maxSize<=MAX_SIZE);
	}

	void put(const T &elem);

	bool offer(const T &elem);

	bool offer(const T &elem, double second);

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

	bool setMaxSize(uint32_t n) {
		if (n > MAX_SIZE) {
			return false;
		}
		MutexLockGuard lockGuard(mutex);
		maxSize = n;
		return true;
	}
	
private:
	std::queue<T> elemQue;

	Mutex mutex;

	Condition noEmptyCond;

	Condition noFullCond;

	uint32_t maxSize;
};

template<typename T>
void BoundedBlockingQueue<T>::put(const T &elem)
{
	{
		MutexLockGuard lockGuard(mutex);
		while (elemQue.size() == maxSize) {
			noFullCond.wait();
		}
		elemQue.push(elem);
	}
	noEmptyCond.notify();
}

template<typename T>
T BoundedBlockingQueue<T>::take()
{
	MutexLockGuard lockGuard(mutex);
	while (elemQue.empty()) {
		noEmptyCond.wait();
	}
	T elem = elemQue.front();
	elemQue.pop();
	noFullCond.notify();
	return elem;
}

template<typename T>
bool BoundedBlockingQueue<T>::poll(T &elem, double second)
{
	MutexLockGuard lockGuard(mutex);
	while (elemQue.empty()) {
		int ret = noEmptyCond.wait(second);
		if (ret == ETIMEDOUT) {
			return false;
		}
	}
	elem = elemQue.front();
	elemQue.pop();
	noFullCond.notify();
	return true;
}

template<typename T>
bool BoundedBlockingQueue<T>::offer(const T &elem)
{
	{
		MutexLockGuard lockGuard(mutex);
		if (elemQue.size() == maxSize) {
			return false;
		}
		elemQue.push(elem);
	}
	noEmptyCond.notify();
	return true;
}

template<typename T>
bool BoundedBlockingQueue<T>::offer(const T &elem, double second)
{
	{
		MutexLockGuard lockGuard(mutex);
		if (elemQue.size() == maxSize) {
			if (noFullCond.wait(second) == ETIMEDOUT) {
				return false;
			}
		}
		elemQue.push(elem);
	}
	noEmptyCond.notify();
	return true;
}

}

#endif

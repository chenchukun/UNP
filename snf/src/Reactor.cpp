#include "Reactor.h"
#include <iostream>

namespace snf
{

__thread Reactor *runInThisThread = NULL;

Reactor::Reactor() 
	: poller(NULL),
	  running(false),
	  threadId(CurrThread::tid()),
	  stoping(false),
	  timerHeap(new TimerHeap)
{
	if (runInThisThread != NULL) {
		abortNotInRunThread();
	}
	runInThisThread = this;
	poller = Poller::create();
	ctlPipe.setNonBlock(true);
	ctlEventHandler.setHandle(ctlPipe.getReadHandle());
	ctlEventHandler.setInCallback(true, std::bind(&Reactor::ctlHandler, this));
	addEvent(ctlEventHandler);
	addEvent(timerHeap->getEventHandler());
//	std::cout << "ctlHandle: " << ctlEventHandler.getHandle() << std::endl;
//	std::cout << "timerHeapHandle: " << timerHeap->getEventHandler().getHandle() << std::endl;
//	poller->addHandle(ctlPipe.getReadHandle(), SNF_POLLIN);
}

void Reactor::ctlHandler()
{
	ctlPipe.clear();
	std::vector<ThreadTask> queue;
	// 可以缩小临界区，同时也可以避免死锁，因为回调可能对mutex加锁
	{
		MutexLockGuard lock(mutex);
		threadTaskQueue.swap(queue);
	}

	for (auto task : queue) {
		task();
	}
}

Reactor::~Reactor() {
	assert(!running);
	assertInRunThread();
	while (stoping);
	auto it = eventMap.begin();
	while (it != eventMap.end()) {
		auto eventHandler = it->second.lock();
		if (eventHandler != NULL) {
			assert(eventHandler->getReactor() == this);
//			eventHandler->setReactor(NULL);
			delEvent(*eventHandler);
		}
		++it;
	}
//	poller->delHandle(ctlPipe.getReadHandle());
	runInThisThread = NULL;
}

Reactor* Reactor::getReactorOfCurrentThread() {
	return runInThisThread;
}

void Reactor::addEvent(EventHandler &eventHandler)
{
	if (isInRunThread()) {
		addEventInReactorThread(eventHandler);
	}
	else {
		MutexLockGuard lock(mutex);
		threadTaskQueue.push_back(std::bind(&Reactor::addEventInReactorThread, this, eventHandler));
		wakeup();
	}
}

void Reactor::addEventInReactorThread(EventHandler &eventHandler)
{
	assert(eventHandler.getReactor() == NULL);
	assert(eventMap.count(eventHandler.getHandle()) == 0);
	int ret = poller->addHandle(eventHandler.getHandle(), eventHandler.getEvents());
//	assert(ret == 0);
	eventMap[eventHandler.getHandle()] = eventHandler.getWeakPtr();
	eventHandler.setReactor(this);
}

void Reactor::delEvent(EventHandler &eventHandler)
{
//	std::cout << "delHandle: " << eventHandler.getHandle() << std::endl;
	if (isInRunThread()) {
		delEventInReactorThread(eventHandler);
	}
	else {
		MutexLockGuard lock(mutex);
		threadTaskQueue.push_back(std::bind(&Reactor::delEventInReactorThread, this, eventHandler));
		wakeup();
	}
}

void Reactor::delEventInReactorThread(EventHandler &eventHandler)
{
	assert(eventHandler.getReactor() == this);
	assert(eventMap.count(eventHandler.getHandle()) == 1); 
	int ret = poller->delHandle(eventHandler.getHandle());
//	assert(ret == 0);
	eventMap.erase(eventHandler.getHandle());
	eventHandler.setReactor(NULL);
}

int Reactor::updateEvent(EventHandler &eventHandler)
{
	assert(eventHandler.getReactor() == this);
	if (eventMap.count(eventHandler.getHandle()) <= 0) {
		return -1;
	}
	int ret = poller->setEvent(eventHandler.getHandle(), eventHandler.getEvents());
	if (ret != 0) {
		return ret;
	}
	return 0;
}

void Reactor::run() 
{
	assert(!running);
	assertInRunThread();
	running = true;
	while (running) {
		int ret = poller->poll();
//		std::cout << "return: " << ret << std::endl;
		returnTimestamp = Timestamp::now();
		if (ret == 0) {
			continue;
		}
		// 出错
		if (ret < 0) {
			break;
		}
		Poller::Iterator it = NULL;
		while ( (it=poller->getActiveIterator()) != NULL) {
			if (eventMap.count(it->handle()) > 0) {
				auto eventHandler = eventMap[it->handle()].lock();
				if (eventHandler == NULL) {
					poller->delHandle(it->handle());
					eventMap.erase(it->handle());
					continue;
				}
				eventHandler->setRevents(it->event());
				eventHandler->handler();
			}
		}
	}
}

// 重点，如何保证安全？poll先退出的话，可能Reacotr被销毁，这是如果pipe还在运行着会导致coredump
void Reactor::stop()
{
	stoping = true;
	assert(running);
	running = false;
	int len;
	if (!isInRunThread()) {
		wakeup();
	}
	stoping = false;
}

TimerId Reactor::runAt(const Timer::TimerHandler &cb, Timestamp timestamp)
{
	Timer *ptimer = new Timer(cb, timestamp, 0);
	TimerId timerId(ptimer, ptimer->getId());
	runInReactorThread(std::bind(&TimerHeap::addTimer, timerHeap.get(), ptimer));
	return timerId;
}

TimerId Reactor::runAfter(const Timer::TimerHandler &cb, double interval)
{
	Timestamp now = Timestamp::now();
	now += static_cast<uint64_t>(interval*Timestamp::TIME_USEC);
	Timer *ptimer = new Timer(cb, now, 0);
	TimerId timerId(ptimer, ptimer->getId());
	runInReactorThread(std::bind(&TimerHeap::addTimer, timerHeap.get(), ptimer));
	return timerId;
}

void Reactor::runInReactorThread(const ThreadTask &task) {
	if (isInRunThread()) {
		task();
	}
	else {
		MutexLockGuard lock(mutex);
		threadTaskQueue.push_back(task);
		wakeup();
	}
}

TimerId Reactor::runEvery(const Timer::TimerHandler &cb, double interval)
{
	Timestamp now = Timestamp::now();
	now += static_cast<uint64_t>(interval*Timestamp::TIME_USEC);
	Timer *ptimer = new Timer(cb, now, interval);
	TimerId timerId(ptimer, ptimer->getId());
	runInReactorThread(std::bind(&TimerHeap::addTimer, timerHeap.get(), ptimer));
	return timerId;
}

void Reactor::wakeup()
{
	if (ctlPipe.empty()) {
		int len;
		while((len=ctlPipe.write(REACTOR_CMD_WAKEUP, 1)) != 1);
	}
}

}

#ifndef REACTOR_H
#define REACTOR_H
#include "Poller.h"
#include "Thread.h"
#include "EventHandler.h"
#include "Uncopyable.h"
#include "Pipe.h"
#include "TimerHeap.h"
#include "Mutex.h"
#include <vector>
#include <map>
#include <cstring>

#define REACTOR_CMD_WAKEUP "0"

namespace snf
{
class Reactor;

class Reactor : public Uncopyable
{
public:
	typedef std::function<void(void)> ThreadTask;

	Reactor();

	~Reactor();

	void addEvent(EventHandler &eventHandler);

	void delEvent(EventHandler &eventHandler);

	int updateEvent(EventHandler &eventHandler);

	TimerId runAt(const Timer::TimerHandler &cb, Timestamp timestamp);

	TimerId runAfter(const Timer::TimerHandler &cb, double interval);

	TimerId runEvery(const Timer::TimerHandler &cb, double interval);

	int cancel(TimerId timerId) {
		return timerHeap->delTimer(timerId);
	}

	void run();

	void stop();

	int size() {
		return eventMap.size();
	}

	void assertInRunThread() {
		if (!isInRunThread()) {
			abortNotInRunThread();
		}
	}

	// 判断当前是否是run线程
	bool isInRunThread() const {
		return threadId == CurrThread::tid();
	}

	static Reactor* getReactorOfCurrentThread();

	void runInReactorThread(const ThreadTask &task);

	Timestamp getReturnTimestamp() {
		return returnTimestamp;
	}

private:
	void abortNotInRunThread() {
		assert(false);
	}

	void addEventInReactorThread(EventHandler &eventHandler);

	void delEventInReactorThread(EventHandler &eventHandler);

	void wakeup();

	void ctlHandler();

	std::shared_ptr<Poller> poller;

	bool running;

	const pid_t threadId;

	std::map<int, std::weak_ptr<EventHandler>> eventMap;

	Pipe ctlPipe;

	bool stoping;

	EventHandler ctlEventHandler;

	std::unique_ptr<TimerHeap> timerHeap;

	std::vector<ThreadTask> threadTaskQueue;

	Timestamp returnTimestamp;

	Mutex mutex;
};

}

#endif

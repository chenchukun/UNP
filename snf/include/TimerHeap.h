#ifndef TIMER_HEAP_H
#define TIMER_HEAP_H
#include "Timer.h"
#include "EventHandler.h"
#include "TimerId.h"
#include <unistd.h>
#include <vector>
#include <sys/timerfd.h>

namespace snf
{

class TimerHeap
{
public:
	TimerHeap();

	~TimerHeap() {
		::close(timerfd);
	}

	EventHandler& getEventHandler();

//	TimerId addTimer(const Timer::TimerHandler &cb, Timestamp when, double interval=0);

	void addTimer(Timer *timer);

	int delTimer(TimerId timerId);

	size_t size() {
		return timers.size();
	}

	void debug();

	void handler();

	void start();

private:

	void readTimer();

	void insertTimer(Timer *ptimer);

	void shiftUp(size_t index);

	void shiftDown(size_t index);

	int createTimer();

	int timerfd;

	EventHandler eventHandler;

	std::vector<std::unique_ptr<Timer>> timers;
};

}

#endif

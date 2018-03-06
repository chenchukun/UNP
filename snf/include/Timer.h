#ifndef TIMER_H
#define TIMER_H
#include <functional>
#include <atomic>
#include "Timestamp.h"

namespace snf
{

class TimerHeap;

class Timer
{
	friend class TimerHeap;
public:
	typedef std::function<void(void)> TimerHandler;

	Timer(const TimerHandler &timerHandler, Timestamp expire, double interval)
		: expireTime(expire),
		  intervalTime(interval),
		  repeat_(interval>0),
		  handler(timerHandler),
	      index(0),
		  id(timerNum++) {}

	Timestamp expire() const {
		return expireTime;
	}

	double interval() const {
		return intervalTime;
	}

	bool repeat() const {
		return repeat_;
	}

	void run() const {
		handler();
	}

	void reset(Timestamp now);

	uint64_t getId() {
		return id;
	}

private:
	Timestamp expireTime;

	double intervalTime;

	bool repeat_;

	TimerHandler handler;
	
	size_t index;

	uint64_t id;	

	static std::atomic<uint64_t> timerNum;
};

}

#endif

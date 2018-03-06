#ifndef TIMER_ID_H
#define TIMER_ID_H
#include "Timer.h"

namespace snf
{

class TimerHeap;

class TimerId
{
	friend class TimerHeap;
public:
	TimerId(): timer(NULL), id(0) {}

	TimerId(Timer *t, uint64_t i)
		: timer(t), id(i) {}

private:
	Timer *timer;

	uint64_t id;
};

}

#endif

#include "TimerHeap.h"
#include "Base.h"
#include <assert.h>
#include <iostream>

namespace snf
{

TimerHeap::TimerHeap()
{
	timerfd = createTimer();
	eventHandler.setHandle(timerfd);
	eventHandler.setInCallback(true, std::bind(&TimerHeap::handler, this));
}

EventHandler& TimerHeap::getEventHandler() 
{
	return eventHandler;
}

int TimerHeap::createTimer()
{
//  int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
    int fd = timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC|TFD_NONBLOCK);
    assert(fd != -1);
    return fd; 
}

void TimerHeap::handler()
{
	readTimer();
	while ( !timers.empty() ) {
		Timestamp topTimestamp = timers[0]->expire();
		Timestamp now = Timestamp::now();
		if (now >= topTimestamp) {
			timers[0]->run();
			if (timers[0]->repeat()) {
				timers[0]->reset(now + static_cast<uint64_t>(timers[0]->interval()*Timestamp::TIME_USEC));
				shiftDown(0);
			}
			else {
				delTimer(TimerId(timers[0].get(), timers[0]->getId()));
			}
		}
		else {
			break;
		}
	}
	start();
}

void TimerHeap::shiftUp(size_t index)
{
	size_t curr = index;
	while ( curr > 0 ) {
		if (timers[curr]->expire() < timers[(curr-1)/2]->expire()) {
			timers[(curr-1)/2]->index = curr;
			swap(timers[curr], timers[(curr-1)/2]);
			curr = (curr-1) / 2;
		}
		else {
			break;
		}
	}
	timers[curr]->index = curr;
}

void TimerHeap::shiftDown(size_t index)
{
	size_t curr = index;
	while (curr*2 + 1 < timers.size()) {
		size_t min = curr;
		if (timers[min]->expire() > timers[curr*2 + 1]->expire()) {
			min = curr*2 + 1;
		}
		if (curr*2 + 2 < timers.size() && timers[min]->expire() > timers[curr*2 + 2]->expire()) {
			min = curr*2 + 2;
		}
		if (min == curr) {
			break;
		}
		timers[min]->index = curr;
		swap(timers[curr], timers[min]);
		curr = min;
	}
	timers[curr]->index = curr;
}

void TimerHeap::insertTimer(Timer *ptimer)
{
	timers.push_back(std::unique_ptr<Timer>(ptimer));
//	ptimer->index = timers.size() -1;
	shiftUp(timers.size() - 1);
}

/*
TimerId TimerHeap::addTimer(const Timer::TimerHandler &cb, Timestamp when, double interval)
{
	Timer *ptimer = new Timer(cb, when, interval);
	insertTimer(ptimer);
	TimerId timerId(ptimer, ptimer->getId());
	start();
	return timerId;
}
*/

void TimerHeap::addTimer(Timer *ptimer)
{
	insertTimer(ptimer);
	start();
}

int TimerHeap::delTimer(TimerId timerId)
{
	size_t index = timerId.timer->index;
	assert(timers[index].get() == timerId.timer);
	if (index == size() -1 ) {
		timers.pop_back();
		return 0;
	}
	Timer *tail = timers[size()-1].release();
	timers[index].reset(tail);
	timers.pop_back();
	shiftDown(index);
	start();
	return 0;
}

void TimerHeap::debug() 
{
	for (int i=0; i<timers.size(); ++i) {
//		std::cout << timers[i]->index << " " << timers[i]->expire().usec() << std::endl;
	}
}

void TimerHeap::start()
{
	if (size() == 0) {
		return ;
	}
	Timestamp topTimestamp = timers[0]->expire();
	struct itimerspec itsp;
	itsp.it_value.tv_sec = topTimestamp.sec();
	itsp.it_value.tv_nsec = (topTimestamp.usec() - topTimestamp.sec()*Timestamp::TIME_USEC) * 1000;
	itsp.it_interval.tv_sec = itsp.it_interval.tv_nsec = 0;
	int ret = timerfd_settime(timerfd, TFD_TIMER_ABSTIME, &itsp, NULL);
	assert(ret != -1);
}

void TimerHeap::readTimer() 
{
	char buff[8];
	int n = 8;
	int len = 0;
	readn(timerfd, buff, 8);
}

}



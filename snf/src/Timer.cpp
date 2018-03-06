#include "Timer.h"

namespace snf
{

std::atomic<uint64_t> Timer::timerNum;

void Timer::reset(Timestamp now) {
	if (repeat_) {
		expireTime = now + intervalTime;
	}
	else {
		expireTime = Timestamp(0);
	}
}
}

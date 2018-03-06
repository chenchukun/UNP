#include "Condition.h"
#include <sys/types.h>

namespace snf
{

int Condition::wait(double second) 
{
	if (second < 0) {
		return -1;
	}
	const int64_t NSEC = 1000000000;
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	int64_t sec = static_cast<int64_t>(second);
	int64_t nsec = static_cast<int64_t>((second-sec)*NSEC);
	spec.tv_sec += static_cast<time_t>(sec + (spec.tv_nsec+nsec)/NSEC);
	spec.tv_nsec += static_cast<long>((spec.tv_nsec+nsec)/NSEC);
	return pthread_cond_timedwait(&cond, &mutex.mutex, &spec);	
}

}

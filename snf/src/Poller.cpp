#include "Poller.h"
#include "Epoll.h"
#include "Poll.h"

namespace snf
{
std::shared_ptr<Poller> Poller::create() 
{
#ifdef USE_SYS_EPOLL
	return std::make_shared<Epoll>();
#else
	return std::make_shared<Poll>();
#endif
}

}


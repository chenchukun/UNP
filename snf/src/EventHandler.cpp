#include "EventHandler.h"
#include "Reactor.h"

namespace snf
{

EventHandler::~EventHandler()
{
	if (reactor != NULL) {
		reactor->delEvent(*this);
		reactor = NULL;
	}
}

void EventHandler::handler()
{
	handlering = true;
	if ((revents & SNF_POLLIN) && handlerIn!=NULL) {
		handlerIn();
	}

	if ((revents & SNF_POLLOUT) && handlerOut!=NULL) {
		handlerOut();
	}

	if ((revents & SNF_POLLPRI) && handlerPri!=NULL) {
		handlerPri();
	}

	if ((revents & SNF_POLLERR) && handlerErr!=NULL) {
		handlerErr();
	}

	if ((revents & SNF_POLLRDHUP) && !(revents & SNF_POLLIN) && handlerHup!=NULL) {
		handlerHup();
	}
	handlering = false;
}

}

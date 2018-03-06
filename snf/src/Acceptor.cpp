#include "Acceptor.h"
#include "SockStream.h"

namespace snf
{

Acceptor::Acceptor(Reactor *reactor) 
	: reactor(reactor),
	  listenning_(false),
	  newConnectionCallback(NULL)
{
}

Acceptor::~Acceptor()
{
	if (listenning_ && reactor!=NULL) {
		reactor->delEvent(eventHandler);
		sockAccept.close();
		reactor = NULL;
	}
}

int Acceptor::listen(const SockAddr &addr, int backlog, int proto)
{
	int ret = sockAccept.open(addr, backlog, proto);
	if (ret != 0) {
		return ret;
	}
	sockAccept.setNonBlock(true);
	int handle = sockAccept.getHandle();
	eventHandler.setHandle(handle);
	eventHandler.setInCallback(true, std::bind(&Acceptor::handler, this));
	reactor->addEvent(eventHandler);
	listenning_ = true;
	return 0;
}

void Acceptor::handler()
{
	for (int i=0; i<ACCEPT_NUM; ++i) {
		SockStream sockStream;
		int ret = sockAccept.accept(sockStream);
		if (ret != 0) {
			break;
		}
		auto poller = Poller::create();
		poller->addHandle(sockStream.getHandle(), SNF_POLLOUT);
		ret = poller->poll(0);
		if (ret <= 0) {
			sockStream.close();
			continue;
		}
		if (newConnectionCallback) {
			newConnectionCallback(sockStream);
		}
		else {
			sockStream.close();
		}
	}
}

}

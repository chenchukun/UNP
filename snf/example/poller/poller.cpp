#include "Poller.h"
#include "SockAccept.h"
#include <iostream>
#include <assert.h>
using namespace snf;
using namespace std;

int main()
{
	auto poller = Poller::create();
	SockAccept sockAccept;
	SockAddr localAddr("0.0.0.0", 6180);
	assert(sockAccept.open(localAddr)==0);
	int listenFd = sockAccept.getHandle();
	poller->addHandle(listenFd, SNF_POLLIN);
	while (true) {
		int n= poller->poll();
		if (n <= 0) {
			break;
		}
		Poller::Iterator it;
		while ((it = poller->getActiveIterator())!=NULL) {
			int handle = it->handle();
			int event = it->event();
			
			if (handle == listenFd) {
				SockStream remoteStream;
				assert(sockAccept.accept(remoteStream)==0);
				assert(poller->addHandle(remoteStream.getHandle(), SNF_POLLIN)==0);
				assert(remoteStream.getPeerAddr(remoteAddr)==0);
			}
		}
	}
	return 0;
}

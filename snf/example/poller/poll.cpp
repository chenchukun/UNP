#include "Poll.h"
#include "Epoll.h"
#include "SockAccept.h"
#include <iostream>
#include <assert.h>
using namespace snf;
using namespace std;

int main()
{
	auto poller = Poller::create();
	int exitCnt = 0;
	SockAccept sockAccept;
	SockAddr localAddr("0.0.0.0", 6180);
	assert(sockAccept.open(localAddr)==0);
	char buff[1024];
	int listenFd = sockAccept.getHandle();
	SockAddr remoteAddr;

//	close(listenFd);
	poller->addHandle(listenFd, SNF_POLLIN);
	cout << "start poll, poller->size() = " << poller->size() << endl;
	while (true) {
		int n = poller->poll(3);
//		cout << "return: " << n << endl;
		if (n == 0) {
			cout << "timeout" << endl;
			cout << endl;
			++exitCnt;
			if (exitCnt >= 10) {
				break;
			}
			continue;
		}
		if (n == -1) {
			cout << "poll error" << endl;
			cout << "  " << errno << endl;
			cout << endl;
			continue;
		}
//		cout << "poll return, activeNum = " << n << endl;
//		cout << "poll return, poller->size() = " << poller->size() << endl;
//		cout << endl;
		Poller::Iterator it;
		while ((it = poller->getActiveIterator())!=NULL) {
			int handle = it->handle();
			int event = it->event();
			cout << "active: handle = " << handle << ", event = " << event << endl;
			cout << endl;
			cout << SNF_POLLIN << endl;
			cout << SNF_POLLPRI << endl;
			cout << SNF_POLLOUT << endl;
			cout << SNF_POLLERR << endl;
			cout << SNF_POLLRDHUP << endl;
//			cout << POLLNVAL << endl;
			SockStream sockStream;
			sockStream.setHandle(handle);

			if (handle == listenFd) {
				SockStream remoteStream;
				assert(sockAccept.accept(remoteStream)==0);
				assert(poller->addHandle(remoteStream.getHandle(), SNF_POLLIN)==0);
				assert(remoteStream.getPeerAddr(remoteAddr)==0);
				cout << "listen: new handle = " << remoteStream.getHandle() << " remote addr = " <<  remoteAddr.format() << endl;
				cout << "poller->size() = " << poller->size() << endl;
				cout << endl;
			}
			else {
				int len = sockStream.read(buff, sizeof(buff));
				if (len == 0 || len == -1) {
					assert(poller->delHandle(handle)==0);
					sockStream.close();
					cout << "del: poller->size() = " << poller->size() << endl;
					cout << endl;
				}
				else {
					sockStream.write(buff, len);
					assert(sockStream.getPeerAddr(remoteAddr)==0);
//					cout << "echo: handle = " << sockStream.getHandle() << " remote addr = " <<  remoteAddr.format() << endl;
//					cout << endl;
				}
			}
		}
	}
	return 0;
}

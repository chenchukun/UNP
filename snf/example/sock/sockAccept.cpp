#include "SockAccept.h"
#include "Thread.h"
#include <memory>
#include <iostream>
#include <functional>
#include <assert.h>
using namespace snf;
using namespace std;

void echo(shared_ptr<SockStream> sockStream)
{
	const int BUFF_SIZE = 1024;
	char buff[BUFF_SIZE];
	int len;
	while (true) {
		len = sockStream->read(buff, BUFF_SIZE);
		if (len == -1) {
			cout << "read error" << endl;
			break;
		}
		if (len == 0) {
			cout << "read end" << endl;
			break;
		}
		sockStream->write(buff, len);
	}
}

int main()
{
	SockAddr localAddr("0.0.0.0", 6180);
	SockAccept sockAccept;

	assert(sockAccept.open(localAddr)==0);
	while (true) {
		shared_ptr<SockStream> sockStream = make_shared<SockStream>();
		int ret = sockAccept.accept(*sockStream, 3.2);
		if (ret != 0) {
			if (errno == ETIMEDOUT) {
				cout << "accept timeout" << endl;
			}
			else {
				cout << "accept error" << endl;
			}
			continue;
		}
		Thread *thread = new Thread(bind(echo, sockStream));
		thread->start();
		thread->detach();
	}
	return 0;
}

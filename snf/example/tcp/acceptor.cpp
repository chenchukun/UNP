#include "Acceptor.h"
#include "Thread.h"
#include <cstring>
#include <iostream>
using namespace std;
using namespace snf;

const char *text = "Hello World!\n";

void thread(SockStream sockStream) 
{
	sockStream.setNonBlock(false);
	cout << "start: " << sockStream.getHandle() << endl;
	char buff[1024];
	while (true) {
		int len = sockStream.read(buff, sizeof(buff));
		if (len <= 0) {
			break;
		}
		sockStream.write(buff, len);
	}
	sockStream.close();
}

void handler(SockStream &sockStream) 
{
	Thread t(bind(thread, sockStream));
	t.start();
}

int main()
{
	Reactor reactor;
	Acceptor acceptor(&reactor);
	acceptor.setNewConnectionCallback(handler);
	SockAddr addr("0.0.0.0", 6180);
	assert(acceptor.listen(addr) == 0);

	Acceptor acceptor2(&reactor);
	acceptor2.setNewConnectionCallback(handler);
	SockAddr addr2("0.0.0.0", 61800);
	assert(acceptor2.listen(addr2) == 0);
	reactor.run();
	return 0;
}

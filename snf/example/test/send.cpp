#include "SockAddr.h"
#include "SockConnect.h"
#include "Thread.h"
#include <assert.h>
using namespace std;
using namespace snf;

bool running = true;

void run()
{
	sleep(5);
	running = false;
}

void sendThread()
{
	SockAddr addr("127.0.0.1", 6180);
	SockConnect sockConnect;
	SockStream sockStream;
	assert(sockConnect.connect(sockStream, addr) == 0);
	char buff[256];
	while (running) {
		sockStream.write(buff, sizeof(buff));
	}
}

int main()
{
	Thread t1(sendThread);
	t1.start();
	Thread t2(sendThread);
	t2.start();
	Thread t3(sendThread);
	t3.start();
	Thread t(run);
	t.start();
	t.join();
	return 0;
}

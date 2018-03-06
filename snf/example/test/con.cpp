#include "TcpServer.h"
#include <atomic>
#include <iostream>
using namespace std;
using namespace snf;

atomic_long cnt(0);

void print() 
{
	cout << "连接数: " << cnt << endl;
}


void onMessage(TcpConnectionPtr connectionPtr, Timestamp timestamp)
{
}

void onConnection(TcpConnectionPtr connectionPtr)
{
	if (connectionPtr->connected()) {
		++cnt;
	}
	else {
		--cnt;
	}
}

int main()
{
	Reactor reactor;
	SockAddr addr("0.0.0.0", 6180);
	TcpServer server(&reactor, addr);
	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.start();
	reactor.runEvery(print, 3);
	reactor.run();
	return 0;
}

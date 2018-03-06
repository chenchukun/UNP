#include "TcpServer.h"
#include <iostream>
using namespace std;
using namespace snf;


// 第二次后无法关闭连接？
void onMessage(TcpConnectionPtr connectionPtr, Timestamp timestamp)
{
	string text;
	connectionPtr->getInputBuffer()->readAll(text);
	connectionPtr->getOutputBuffer()->append(text);
}

void onConnection(TcpConnectionPtr connectionPtr)
{
	if (connectionPtr->connected()) {
		connectionPtr->getSockStream().setNodelay(true);
		cout << "online" << endl;
	}
	else {
//		connectionPtr->shutdown();
		cout << "offline" << endl;
	}
}

int main()
{
	Reactor reactor;
	SockAddr addr("127.0.0.1", 6180);
	TcpServer server(&reactor, addr);
	server.setConnectionCallback(onConnection);
	server.setMessageCallback(onMessage);
	server.start();
	reactor.run();
	return 0;
}

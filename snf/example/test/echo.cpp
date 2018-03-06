#include "TcpServer.h"
#include <iostream>
using namespace std;
using namespace snf;


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
		SockAddr peerAddr = connectionPtr->getPeerAddr();
		cout << peerAddr.format() << " online" << endl;
	}
	else {
		SockAddr peerAddr = connectionPtr->getPeerAddr();
		cout << peerAddr.format() << " offline" << endl;
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
	reactor.run();
	return 0;
}

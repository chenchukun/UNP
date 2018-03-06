#include "TcpServer.h"
#include <iostream>
using namespace std;
using namespace snf;

Reactor *gReactor;
int cnt = 0;

void stop()
{
	gReactor->stop();
}

void onMessage(TcpConnectionPtr connectionPtr, Timestamp timestamp)
{
	/*
	string text;
	connectionPtr->getInputBuffer()->readAll(text);
	connectionPtr->getOutputBuffer()->append(text);
	*/
}

void onConnection(TcpConnectionPtr connectionPtr)
{
	if (connectionPtr->connected()) {
		cout << connectionPtr->getPeerAddr().format() << " online" << endl;
	}
	else {
		cout << connectionPtr->getPeerAddr().format() << " offline" << endl;
		connectionPtr->shutdown();
		++cnt;
	}
}

void onWriteComplete(TcpConnectionPtr connectionPtr)
{
	cout << "onWriteComplete" << endl;
}

void onError(TcpConnectionPtr connectionPtr)
{
	cout << "onError" << endl;
}


int main()
{
	Reactor reactor;
	gReactor = &reactor;
	SockAddr addr("0.0.0.0", 6180);
	TcpServer server(&reactor, addr);
	server.setMessageCallback(onMessage);
	server.setConnectionCallback(onConnection);
	server.setWriteCompleteCallback(onWriteComplete);
	server.setErrorCallback(onError);
	server.start();
	reactor.runAfter(stop, 100);
	reactor.run();
	cout << "cnt: " << cnt << endl;
	return 0;
}

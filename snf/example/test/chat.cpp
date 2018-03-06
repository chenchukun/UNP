#include "TcpServer.h"
#include <vector>
#include <iostream>
using namespace std;
using namespace snf;

map<int, TcpConnectionPtr> clientMap;

void onMessage(TcpConnectionPtr connectionPtr, Timestamp timestamp)
{
	string text;
	connectionPtr->getInputBuffer()->readAll(text);
	auto it = clientMap.cbegin();
	while (it != clientMap.end()) {
		if (it->first != connectionPtr->getSockStream().getHandle()) {
			it->second->send(text);
		}
		++it;
	}
}

void onConnection(TcpConnectionPtr connectionPtr)
{
	if (connectionPtr->connected()) {
		connectionPtr->getSockStream().setNodelay(true);
		SockAddr peerAddr = connectionPtr->getPeerAddr();
		cout << peerAddr.format() << " online" << endl;
		clientMap[connectionPtr->getSockStream().getHandle()] = connectionPtr;
	}
	else {
		SockAddr peerAddr = connectionPtr->getPeerAddr();
		cout << peerAddr.format() << " offline" << endl;
		clientMap.erase(connectionPtr->getSockStream().getHandle());
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

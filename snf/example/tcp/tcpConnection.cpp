#include "Acceptor.h"
#include "TcpConnection.h"
#include "Thread.h"
#include <cstring>
#include <iostream>
using namespace std;
using namespace snf;

Reactor *gReactor;
vector<TcpConnectionPtr> connections;

void onMessage(TcpConnectionPtr connectionPtr, Timestamp timestamp)
{
	std::string text;
//	connectionPtr->getInputBuffer()->readAll(text);
//	cout << text << endl;
//	connectionPtr->getOutputBuffer()->append(text);
}

void onClose(TcpConnectionPtr connectionPtr)
{
	cout << connectionPtr->getPeerAddr().format() << " offline" << endl;
	connectionPtr->shutdown();
}

void handler(SockStream &sockStream) 
{
	TcpConnectionPtr connection = make_shared<TcpConnection>(gReactor, sockStream);
	connection->setMessageCallback(onMessage);
	connection->setCloseCallback(onClose);
	connections.push_back(connection);
}

int main()
{
	Reactor reactor;
	gReactor = &reactor;
	Acceptor acceptor(&reactor);
	acceptor.setNewConnectionCallback(handler);
	SockAddr addr("0.0.0.0", 6180);
	assert(acceptor.listen(addr) == 0);

	reactor.run();
	return 0;
}

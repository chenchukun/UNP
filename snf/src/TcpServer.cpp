#include "TcpServer.h"
#include <iostream>

namespace snf
{

TcpServer::TcpServer(Reactor *reactor, const SockAddr &addr)
	: reactor(reactor),
	  acceptor(reactor),
	  sockAddr(addr),
	  connectionCallback(NULL),
	  messageCallback(NULL),
	  writeCompleteCallback(NULL),
	  errorCallback(NULL)
{
}

void TcpServer::setConnectionCallback(const ConnectionCallback &cb)
{
	connectionCallback = cb;
}

void TcpServer::setMessageCallback(const MessageCallback &cb)
{
	messageCallback = cb;
}

void TcpServer::setWriteCompleteCallback(const WriteCompleteCallback &cb)
{
	writeCompleteCallback = cb;
}

void TcpServer::setErrorCallback(const ErrorCallback &cb)
{
	errorCallback = cb;
}

void TcpServer::start()
{
	acceptor.setNewConnectionCallback(std::bind(&TcpServer::newConnectionCallback, this, std::placeholders::_1));
	assert(acceptor.listen(sockAddr) == 0);
//	std::cout << "acceptorHandle: " << acceptor.getSockAccept().getHandle() << std::endl;
}

void TcpServer::newConnectionCallback(SockStream &sockStream)
{
	TcpConnectionPtr connection = std::make_shared<TcpConnection>(reactor, sockStream);
	connectionMap[sockStream.getHandle()] = connection;
	connection->setMessageCallback(messageCallback);
	connection->setWriteCompleteCallback(writeCompleteCallback);
	connection->setErrorCallback(errorCallback);
	connection->setCloseCallback(std::bind(&TcpServer::closeCallback, this, std::placeholders::_1));
	if (connectionCallback) {
		connectionCallback(connection);
	}
}
void TcpServer::closeCallback(TcpConnectionPtr connectionPtr)
{
	removeConnection(connectionPtr);
	connectionCallback(connectionPtr);
}

void TcpServer::removeConnection(TcpConnectionPtr connectionPtr)
{
	int fd = connectionPtr->getSockStream().getHandle();
	if (connectionMap.count(fd) > 0) {
		connectionMap.erase(fd);
	}
}

}

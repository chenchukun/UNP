#include "TcpClient.h"

namespace snf
{

TcpClient::TcpClient(Reactor *reactor, const SockAddr &addr)
	: reactor(reactor),
	  sockAddr(addr),
	  tcpConnectionPtr(NULL),
	  connectionCallback(NULL),
	  messageCallback(NULL),
	  writeCompleteCallback(NULL),
	  errorCallback(NULL)
{
}

void TcpClient::setConnectionCallback(const ConnectionCallback &cb)
{
	connectionCallback = cb;
}

void TcpClient::setMessageCallback(const MessageCallback &cb)
{
	messageCallback = cb;
}

void TcpClient::setWriteCompleteCallback(const WriteCompleteCallback &cb)
{
	writeCompleteCallback = cb;
}

void TcpClient::setErrorCallback(const ErrorCallback &cb)
{
	errorCallback = cb;
}

void TcpClient::connect() 
{
	int ret = sockConnect.connect(sockStream, sockAddr);
	if (ret == 0) {
		tcpConnectionPtr = std::make_shared<TcpConnection>(reactor, sockStream);
		tcpConnectionPtr->setMessageCallback(messageCallback);
		tcpConnectionPtr->setWriteCompleteCallback(writeCompleteCallback);
		tcpConnectionPtr->setErrorCallback(errorCallback);
		tcpConnectionPtr->setCloseCallback(std::bind(&TcpClient::closeCallback, this, std::placeholders::_1));
		if (connectionCallback) {
			connectionCallback(tcpConnectionPtr);
		}
	}
}


TcpConnectionPtr TcpClient::getConnection()
{
	return tcpConnectionPtr;
}

void TcpClient::newConnectionCallback(SockStream &sockStream)
{
}

void TcpClient::closeCallback(TcpConnectionPtr connectionPtr)
{
}

}

#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include "TcpConnection.h"
#include "SockConnect.h"
#include "Callbacks.h"
#include "Uncopyable.h"
#include <map>

namespace snf
{

class TcpClient : public Uncopyable
{
public:
	TcpClient(Reactor *reactor, const SockAddr &addr);

	void setConnectionCallback(const ConnectionCallback &cb);

	void setMessageCallback(const MessageCallback &cb);

	void setWriteCompleteCallback(const WriteCompleteCallback &cb);

	void setErrorCallback(const ErrorCallback &cb);

	void connect();

	TcpConnectionPtr getConnection();

private:
	void newConnectionCallback(SockStream &sockStream);

	void closeCallback(TcpConnectionPtr connectionPtr);

	Reactor *reactor;

	SockAddr sockAddr;

	SockConnect sockConnect;

	SockStream sockStream;

	TcpConnectionPtr tcpConnectionPtr;

	ConnectionCallback connectionCallback;

	MessageCallback messageCallback;

	WriteCompleteCallback writeCompleteCallback;

	ErrorCallback errorCallback;

};

}

#endif

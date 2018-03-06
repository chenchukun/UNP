#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include "TcpConnection.h"
#include "Callbacks.h"
#include "Acceptor.h"
#include "Uncopyable.h"
#include <map>

namespace snf
{

class TcpServer : public Uncopyable
{
public:
	TcpServer(Reactor *reactor, const SockAddr &addr);

	void setConnectionCallback(const ConnectionCallback &cb);

	void setMessageCallback(const MessageCallback &cb);

	void setWriteCompleteCallback(const WriteCompleteCallback &cb);

	void setErrorCallback(const ErrorCallback &cb);

	void start();

	void removeConnection(TcpConnectionPtr connectionPtr);

private:
	void newConnectionCallback(SockStream &sockStream);

	void closeCallback(TcpConnectionPtr connectionPtr);

	Reactor *reactor;

	Acceptor acceptor;

	SockAddr sockAddr;

	std::map<int, TcpConnectionPtr> connectionMap;

	ConnectionCallback connectionCallback;

	MessageCallback messageCallback;

	WriteCompleteCallback writeCompleteCallback;

	ErrorCallback errorCallback;

};

}

#endif

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include "Reactor.h"
#include "TcpServer.h"
#include "HttpRequest.h"
#include "HttpContext.h"
#include "HttpResponse.h"
#include "ThreadPool.h"

class HttpServer
{
public:
	typedef std::function<void(HttpRequest*, HttpResponse&)> RequestCallback;

	HttpServer(snf::Reactor *reactor, const snf::SockAddr &addr)
		: tcpServer(reactor, addr),
		  requestCallback(NULL),
		  threadPool(4)
	{
		threadPool.start();
	}

	void setRequestCallback(const RequestCallback &cb) {
		requestCallback = cb;
	}

	void start();

private:
	void onMessage(snf::TcpConnectionPtr connectionPtr, snf::Timestamp timestamp);

	void onConnection(snf::TcpConnectionPtr connectionPtr);

	void badRequest(snf::TcpConnectionPtr connectionPtr);

	void work(snf::TcpConnectionPtr connectionPtr);

	snf::TcpServer tcpServer;

	RequestCallback requestCallback;

	snf::ThreadPool threadPool;

};

#endif

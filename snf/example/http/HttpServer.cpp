#include "HttpServer.h"
#include <iostream>
using namespace snf;

void HttpServer::start() {
	tcpServer.setMessageCallback(bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	tcpServer.setConnectionCallback(bind(&HttpServer::onConnection, this, std::placeholders::_1));
	tcpServer.start();
}

void HttpServer::onMessage(TcpConnectionPtr connectionPtr, Timestamp timestamp) {
	threadPool.run(std::bind(&HttpServer::work, this, connectionPtr));
}

void HttpServer::work(snf::TcpConnectionPtr connectionPtr)
{
	Any &context = connectionPtr->getContext();
	HttpContext httpContext;
	assert(context.castTo(httpContext) == 0);
	int ret = httpContext.parse(connectionPtr->getInputBuffer());
	if (ret == -1) {
		badRequest(connectionPtr);
		connectionPtr->shutdown();
	}
	else if (ret == 0) {
		if (requestCallback != NULL) {
			HttpResponse response;
			requestCallback(httpContext.getHttpRequest(), response);
			std::string resp;
			assert(response.serialize(resp)==0);
			connectionPtr->send(resp);
			connectionPtr->shutdown();
		}
	}
}

void HttpServer::onConnection(TcpConnectionPtr connectionPtr) {
	if (connectionPtr->connected()) {
		connectionPtr->setContext(HttpContext());
	}
	else {
	}
}

void HttpServer::badRequest(TcpConnectionPtr connectionPtr) {
	HttpResponse response;
	response.setVersion("HTTP/1.1");
	response.setStatusCode(400);
	response.setBody("400 Bad Request");
	std::string content;
	assert(response.serialize(content)==0);
	connectionPtr->send(content);
}


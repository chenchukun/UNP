#include "TcpConnection.h"
#include <iostream>

namespace snf
{

TcpConnection::TcpConnection(Reactor *reactor, SockStream &sockStream)
	: reactor(reactor),
	  messageCallback(NULL),
	  writeCompleteCallback(NULL),
	  errorCallback(NULL),
	  sockStream(sockStream),
	  eventHandler(sockStream.getHandle()),
	  state(CLOSED)
{
	assert(reactor != NULL);
	assert(sockStream.getLocalAddr(localAddr) == 0);
	assert(sockStream.getPeerAddr(peerAddr) == 0);
	eventHandler.setInCallback(true, std::bind(&TcpConnection::handlerRead, this));
//	eventHandler.setOutCallback(true, std::bind(&TcpConnection::handlerWrite, this));
//	eventHandler.setHupCallback(true, std::bind(&TcpConnection::handlerClose, this));
	eventHandler.setErrCallback(true, std::bind(&TcpConnection::handlerError, this));
	reactor->addEvent(eventHandler);
	setState(CONNECTED);
}

TcpConnection::~TcpConnection()
{
	sockStream.close();
//	std::cout << "~TcpConnection" << std::endl;
}

void TcpConnection::setMessageCallback(const MessageCallback &cb)
{
	messageCallback = cb;
}

void TcpConnection::setWriteCompleteCallback(const WriteCompleteCallback &cb)
{
	writeCompleteCallback = cb;
}

void TcpConnection::setErrorCallback(const ErrorCallback &cb)
{
	errorCallback = cb;
}

void TcpConnection::setCloseCallback(const ConnectionCallback &cb)
{
	closeCallback = cb;
}

void TcpConnection::handlerRead()
{
	int n = inputBuffer.readFd(sockStream.getHandle());
	if (n > 0 && messageCallback) {
		messageCallback(shared_from_this(), reactor->getReturnTimestamp());
		enableWrite();
	}
	else if(n == 0) {
		handlerClose();
	}
	else {
		handlerError();
	}
	/*
	assert(state != RCLOSE);
	assert(state != CLOSED);
	int savedErrno = 0;
	ssize_t n = inputBuffer.readFd(sockStream.getHandle(), &savedErrno);
	if (n > 0 && messageCallback) {
		messageCallback(shared_from_this(), reactor->getReturnTimestamp());
	} 
	else if (n == 0) {
    	handlerClose();
		std::cout << "close" << std::endl;
	} 
	else {
    	errno = savedErrno;
		handlerError();
  	}
	*/
}

void TcpConnection::handlerWrite()
{
	int n = outputBuffer.writeFd(sockStream.getHandle());
	if (n < 0) {
		handlerError();
	}
	if (outputBuffer.size() == 0) {
		eventHandler.setOutCallback(false);
		reactor->updateEvent(eventHandler);
		if (writeCompleteCallback) {
			writeCompleteCallback(shared_from_this());
		}
	}
	/*
//	loop_->assertInLoopThread();
	ssize_t n = ::write(sockStream.getHandle(), outputBuffer.peek(), outputBuffer.readableBytes());
	if (n < 0) {
//		handlerError();
		handlerClose();
		std::cout << "error " << n << std::endl;
//			sockStream.close();
	}
	else if (n > 0) {
		outputBuffer.retrieve(n);
		enableWrite();
	} 
	if (outputBuffer.readableBytes() == 0) {
		eventHandler.setOutCallback(false);
		reactor->updateEvent(eventHandler);
		if (writeCompleteCallback) {
			writeCompleteCallback(shared_from_this());
		}
	}
	*/
}

void TcpConnection::handlerClose()
{
	assert(state != CLOSED);
	if (state == CONNECTED) {
		setState(RCLOSE);
	}
	else {
		setState(LCLOSE);
	}
	if (closeCallback) {
		closeCallback(shared_from_this());
//		std::cout << "handler close" << std::endl;
//		enableWrite();
	}
}

void TcpConnection::handlerError()
{
	if (errorCallback) {
		errorCallback(shared_from_this());
		enableWrite();
	}
}

void TcpConnection::enableWrite() 
{
	if (outputBuffer.size() > 0) {
		eventHandler.setOutCallback(true, std::bind(&TcpConnection::handlerWrite, this));
		reactor->updateEvent(eventHandler);
	}
	/*
	if (outputBuffer.readableBytes() > 0) {
		eventHandler.setOutCallback(true, std::bind(&TcpConnection::handlerWrite, this));
		reactor->updateEvent(eventHandler);
	}
	*/
}

void TcpConnection::send(const std::string &message)
{
	int len = sockStream.write(message.data(), message.size());
	if (len < message.size()) {
		outputBuffer.append(message.data()+len, message.size()-len);
		enableWrite();
	}
	/*
	ssize_t nwrote = 0;
	if (outputBuffer.readableBytes() == 0) {
		nwrote = ::write(sockStream.getHandle(), message.data(), message.size());
  	} 
	assert(nwrote >= 0);
  	if (static_cast<size_t>(nwrote) < message.size()) {
		outputBuffer.append(message.data()+nwrote, message.size()-nwrote);
		enableWrite();
    }
	*/
}

void TcpConnection::shutdown() {
	assert(state != CLOSED);
	assert(state != LCLOSE);
	sockStream.shutdownWrite();
	if (state == RCLOSE) {
		setState(CLOSED);
		reactor->delEvent(eventHandler);
	}
	else {
		setState(LCLOSE);
	}
}

}



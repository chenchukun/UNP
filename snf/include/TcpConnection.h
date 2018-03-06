#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H
#include "Callbacks.h"
#include "Reactor.h"
#include "EventHandler.h"
#include "SockStream.h"
#include "SockAddr.h"
#include "Buffer.h"
#include "Uncopyable.h"
#include "Any.h"

namespace snf
{

class TcpConnection : public std::enable_shared_from_this<TcpConnection>, public Uncopyable
{
public:
	enum State {CONNECTED, RCLOSE, LCLOSE, CLOSED};

	TcpConnection(Reactor *reactor, SockStream &sockStream);

	~TcpConnection();

	void setMessageCallback(const MessageCallback &cb);

	void setWriteCompleteCallback(const WriteCompleteCallback &cb);

	void setErrorCallback(const ErrorCallback &cb);

	void setCloseCallback(const ConnectionCallback &cb);

	Buffer* getInputBuffer() {
		return &inputBuffer;
	}

	Buffer* getOutputBuffer() {
		return &outputBuffer;
	}

	SockAddr getLocalAddr() {
		return localAddr;
	}

	SockAddr getPeerAddr() {
		return peerAddr;
	}

	void shutdown();

	bool connected() {
		return state == CONNECTED;
	}

	void enableWrite();

	SockStream getSockStream() {
		return sockStream;
	}

	void send(const std::string &message);

	void setState(int _state) {
		state = _state;
	}

	Any& getContext() {
		return context;
	}

	void setContext(const Any &cont) {
		context = cont;
	}

private:
	void handlerRead();

	void handlerWrite();

	void handlerClose();

	void handlerError();

	Reactor *reactor;

	MessageCallback messageCallback;

	WriteCompleteCallback writeCompleteCallback;

	ErrorCallback errorCallback;

	ConnectionCallback closeCallback;

	SockStream sockStream;

	EventHandler eventHandler;

	SockAddr localAddr;

	SockAddr peerAddr;

	Buffer inputBuffer;

	Buffer outputBuffer;

	int state;

	Any context;
};

}

#endif

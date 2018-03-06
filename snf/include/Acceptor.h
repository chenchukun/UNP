#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include "SockAccept.h"
#include "EventHandler.h"
#include "Reactor.h"
#include "Uncopyable.h"

namespace snf
{

class Acceptor : public Uncopyable
{
public:
	enum {ACCEPT_NUM=10};

	typedef std::function<void(SockStream&)> NewConnectionCallback;

	Acceptor(Reactor *reactor);

	~Acceptor();

	int listen(const SockAddr &addr, int backlog=100, int proto=0);

	bool listenning() const {
		return listenning_;
	}

	void setNewConnectionCallback(const NewConnectionCallback &cb) {
		newConnectionCallback = cb;
	}

	SockAccept getSockAccept() {
		return sockAccept;
	}


private:
	void handler();

	Reactor *reactor;

	EventHandler eventHandler;

	SockAccept sockAccept;

	bool listenning_;

	NewConnectionCallback newConnectionCallback;

};

}

#endif

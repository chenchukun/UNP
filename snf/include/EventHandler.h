#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
#include <functional>
#include "Poller.h"

namespace snf
{

class Reactor;

class EventHandler
{
public:
	typedef std::function<void(void)> Handler;

	inline EventHandler();

	~EventHandler();

	inline EventHandler(int fd);

	inline void setHandle(int fd);

	inline int getHandle() const;

	inline void setInCallback(bool enable, const Handler &handler=NULL);

	inline void setOutCallback(bool enable, const Handler &handler=NULL);

	inline void setPriCallback(bool enable, const Handler &handler=NULL);

	inline void setErrCallback(bool enable, const Handler &handler=NULL);

	inline void setHupCallback(bool enable, const Handler &handler=NULL);

	inline void setEvents(int event);

	inline int getEvents() const;

	inline void setRevents(int event);

	inline int getRevents() const;

	void handler();

	std::weak_ptr<EventHandler> getWeakPtr() const {
		alive.reset(const_cast<EventHandler*>(this), [](EventHandler *p){});
		return std::weak_ptr<EventHandler>(alive);
	}

	inline void setReactor(Reactor *r) const;

	inline Reactor* getReactor() const;

private:
	int handle;

	Handler handlerIn;

	Handler handlerOut;

	Handler handlerPri;

	Handler handlerErr;

	Handler handlerHup;
	
	int events;

	int revents;

	mutable std::shared_ptr<EventHandler> alive;	

	mutable Reactor *reactor;

	bool handlering;
};

EventHandler::EventHandler()
	: handle(-1),
	  handlerIn(NULL),
	  handlerOut(NULL),
	  handlerPri(NULL),
	  handlerErr(NULL),
	  handlerHup(NULL),
	  events(0),
	  revents(0),
	  reactor(NULL),
	  handlering(false) {}

EventHandler::EventHandler(int fd)
	: handle(fd),
	  handlerIn(NULL),
	  handlerOut(NULL),
	  handlerPri(NULL),
	  handlerErr(NULL),
	  handlerHup(NULL),
	  events(0),
	  revents(0),
	  reactor(NULL) {}

void EventHandler::setHandle(int fd) 
{
	handle = fd;
}
int EventHandler::getHandle() const
{
	return handle;
}

void EventHandler::setInCallback(bool enable, const Handler &handler)
{
	int flags = enable?SNF_POLLIN:0;
	events |= flags;
	handlerIn = handler;
}

void EventHandler::setOutCallback(bool enable, const Handler &handler)
{
	int flags = enable?SNF_POLLOUT:0;
	events |= flags;
	handlerOut = handler;
}

void EventHandler::setPriCallback(bool enable, const Handler &handler)
{
	int flags = enable?SNF_POLLPRI:0;
	events |= flags;
	handlerPri = handler;
}

void EventHandler::setErrCallback(bool enable, const Handler &handler)
{
	int flags = enable?SNF_POLLERR:0;
	events |= flags;
	handlerErr = handler;
}

void EventHandler::setHupCallback(bool enable, const Handler &handler)
{
	int flags = enable?SNF_POLLRDHUP:0;
	events |= flags;
	handlerHup = handler;
}

inline void EventHandler::setEvents(int event)
{
	events = event;
}

inline int EventHandler::getEvents() const
{
	return events;
}

inline void EventHandler::setRevents(int event)
{
	revents = event;
}

inline int EventHandler::getRevents() const
{
	return revents;
}

inline void EventHandler::setReactor(Reactor *r) const
{
	reactor = r;
}

inline Reactor* EventHandler::getReactor() const
{
	return reactor;
}

}

#endif

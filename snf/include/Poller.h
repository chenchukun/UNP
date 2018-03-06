#ifndef POLLER_H
#define POLLER_H
#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <memory>

/*
	POLLIN：可读普通数据
	POLLPRI：可读高优先级数据(TCP带外数据)
	POLLRDHUP：对端套接字关闭（必须定义_GNU_SOURCE测试宏）
	POLLOUT：可写普通数据
	POLLERR：有错误发生
	POLLHUP：出现挂断
	POLLNVAL：文件描述符未打开

	EPOLLIN：普通数据可读
	EPOLLPRI：高优先级数据可读(TCP带外数据)
	EPOLLRDHUP：套接字对端关闭(需要通过epoll_ctl设置吗？)
	EPOLLOUT：普通数据可写
	EPOLLET：使用边缘触发模式
	EPOLLONESHOT：在完成事件通知之后禁用检查（可使用EPOLL_CTL_MOD重新激活）
	EPOLLERR：有错误发生
	EPOLLHUP：出现挂断
*/

#ifdef USE_SYS_EPOLL

	#include <sys/epoll.h>
	#define SNF_POLLIN		EPOLLIN
	#define SNF_POLLPRI		EPOLLPRI	
	#define SNF_POLLOUT		EPOLLOUT
	#define SNF_POLLRDHUP	EPOLLRDHUP
	#define SNF_POLLERR		EPOLLERR
	#define SNF_POLLET		EPOLLET

#else

	#include <poll.h>
	#define SNF_POLLIN		POLLIN
	#define SNF_POLLPRI		POLLPRI	
	#define SNF_POLLOUT		POLLOUT
	#define SNF_POLLRDHUP	POLLRDHUP
	#define SNF_POLLERR		POLLERR
	#define SNF_POLLNVAL	POLLNVAL

#endif

#define SET_EVENT(name, value) void name(bool op) {event = op?event|value:event&~value;}

#define GET_EVENT(name, value) bool name() const {return event & value;}

namespace snf
{

#ifdef USE_SYS_EPOLL
	class Epoll;
#else
	class Poll;
#endif


class PollerIterator
{
public:
	virtual ~PollerIterator() {}

	virtual int handle() = 0;

	virtual int event() = 0;

	/*
	virtual PollerIterator& operator++() = 0;

	virtual bool operator==(const PollerIterator &right) = 0;

	virtual bool operator!=(const PollerIterator &right) = 0;
	*/
};


class Poller
{
public:
	using Iterator = PollerIterator*;

	virtual ~Poller() {}
	
	virtual int poll(double sec=-1) = 0;

	virtual int addHandle(int handle, int event) = 0;

	virtual int delHandle(int handle) = 0;

	virtual int setEvent(int handle, int event) = 0;

	virtual int getEvent(int handle) = 0;

	virtual PollerIterator* getActiveIterator() = 0;

	virtual PollerIterator* getIterator(int index) = 0;

	virtual int size() = 0;

	static std::shared_ptr<Poller> create();

private:
};

}

#endif

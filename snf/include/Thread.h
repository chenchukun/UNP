#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include "Uncopyable.h"
#include <functional>
#include <sys/syscall.h>
#include <unistd.h>
#include <assert.h>
#include <memory>

namespace snf
{

inline pid_t gettid() {
	return syscall(SYS_gettid);
}

namespace CurrThread
{
	extern __thread pid_t currTid;
	inline pid_t tid() {
		if (currTid == 0) {
			currTid = gettid();
		}
		return currTid;
	}
}

class Thread : public Uncopyable
{
public:
	typedef std::function<void(void)> ThreadFun;

	static void* threadRun(void *arg);

	Thread(const ThreadFun &threadFun)
		: threadFun(threadFun),
	 	  _start(false),
		  _join(false),
		  _detach(false),
		  palive(new bool(false)),
		  ptid(new pid_t(0))
	{
		assert(threadFun != NULL);		
	}

	~Thread();

	void start();

	void join();

	void detach();

	pid_t getTid() {
		return *ptid;
	}

	bool isAlive() {
		return *palive;
	}

	bool isStart() {
		return _start;
	}

	bool isJoin() {
		return _join;
	}

	bool isDetach() {
		return _detach;
	}

private:
	struct ThreadData {
		ThreadFun threadFun;
		std::weak_ptr<pid_t> ptid;
		std::weak_ptr<bool> palive;
	};
	pthread_t thread;

	ThreadFun threadFun;

	std::shared_ptr<pid_t> ptid;

	bool _start;

	bool _join;

	bool _detach;

	std::shared_ptr<bool> palive;
};

}

#endif

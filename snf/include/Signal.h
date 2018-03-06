#ifndef SIGNAL_H
#define SIGNAL_H
#include <functional>
#include <signal.h>
#include <map>

namespace snf
{

class Signal
{
public:
	typedef std::function<void(int)> SigCallback;

	static void sigHandler(int sig) {
		if (handlerMap.count(sig)) {
			handlerMap[sig](sig);
		}
	}

	Signal(){}

	~Signal(){}
	
	int set(int sig, const SigCallback &cb, sigset_t set, int flag=0);

	int set(int sig, const SigCallback &cb, int flag=0);

	int setIgnore(int sig);

	int setDefault(int sig);

	int ignoreAll();

	int defaultAll();

private:
	static std::map<int, SigCallback> handlerMap;
};

}

#endif

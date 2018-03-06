#include "Signal.h"
#include <assert.h>
#include <cstdio>

namespace snf
{

std::map<int, Signal::SigCallback> Signal::handlerMap;

int Signal::set(int sig, const SigCallback &cb, sigset_t set, int flag)
{
	sigset_t emptyset;
	sigemptyset(&emptyset);
	struct sigaction act;
	act.sa_handler = sigHandler;
	sigandset(&act.sa_mask, &set, &emptyset);
	act.sa_flags = flag;
	handlerMap[sig] = cb;
	return sigaction(sig, &act, NULL);
}

int Signal::set(int sig, const SigCallback &cb, int flag)
{
	sigset_t emptyset;
	sigemptyset(&emptyset);
	return set(sig, cb, emptyset, flag);
}

int Signal::setIgnore(int sig)
{
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (handlerMap.count(sig)) {
		handlerMap.erase(sig);
	}
	return sigaction(sig, &act, NULL);
}

int Signal::setDefault(int sig)
{
	struct sigaction act;
	act.sa_handler = SIG_DFL;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (handlerMap.count(sig)) {
		handlerMap.erase(sig);
	}
	return sigaction(sig, &act, NULL);
}

int Signal::ignoreAll()
{
	for (int i=1; i<32; ++i) {
		if (i == SIGSTOP || i == SIGKILL) {
			continue;
		}
		int ret = setIgnore(i);
		if (ret != 0) {
			return -1;
		}
	}
	return 0;
}

int Signal::defaultAll() 
{
	for (int i=1; i<32; ++i) {
		if (i == SIGSTOP || i == SIGKILL) {
			continue;
		}
		int ret = setDefault(i);
		if (ret != 0) {
			return -1;
		}
	}
	return 0;
}

}

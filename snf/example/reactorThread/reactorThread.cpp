#include "ReactorThread.h"
#include <iostream>
using namespace std;
using namespace snf;

Reactor *reactor;

void fun()
{
	ReactorThread reactorThread;
	reactor = reactorThread.start();
}

void timeout()
{
	cout << "timeout" << endl;
}

int main()
{
	fun();
	reactor->runEvery(timeout, 1);
	sleep(10);
	cout << "stop" << endl;
	reactor->stop();
	return 0;
}

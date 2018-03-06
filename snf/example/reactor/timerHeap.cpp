#include "TimerHeap.h"
#include <iostream>
using namespace std;
using namespace snf;

void fun()
{
	cout << "timeout" << endl;
}

int main()
{
	TimerHeap timerHeap;
	Timer *ptimer = new Timer(fun, Timestamp::now() + 500000, 0);
	timerHeap.addTimer(ptimer);
	sleep(3);
	EventHandler eventHandler = timerHeap.getEventHandler();
	int handle = eventHandler.getHandle();
	cout << handle << endl;
	char buff[10];
	int n = read(handle, buff, 8);
	cout << n << endl;
	sleep(3);
	return 0;
}

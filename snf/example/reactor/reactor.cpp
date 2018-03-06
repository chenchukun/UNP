#include "Reactor.h"
#include <iostream>
using namespace std;
using namespace snf;
Reactor reactor;
EventHandler eventHandler(0);

void every()
{
	cout << "every 1.9 every: " << Timestamp::now().usec() << endl;
}

void stop()
{
	auto id = reactor.runEvery(every, 1.9);
	sleep(10);
	cout << "stop thread, threadId = " << CurrThread::tid() << endl;
	for (int i=0; i<10; ++i) {
		reactor.runInReactorThread([](){cout << "thread task, threadId = " << CurrThread::tid() << endl;});
	}
	reactor.cancel(id);
	sleep(5);
	reactor.stop();
}

void handler(int handle) 
{
	char buff[128];
	int len = read(handle, buff, sizeof(buff));
	buff[len] = 0;
	cout << buff << endl;
	cout << "return timestamp: " << reactor.getReturnTimestamp().usec() << endl;
}

void timerHandler1() {
	cout << "at + 500000 handler1: " << Timestamp::now().usec() << endl;
}

void timerHandler2() {
	cout << "after 1.2 handler2: " << Timestamp::now().usec() << endl;
}

void timerHandler3() {
	cout << "every 1.5 handler3: " << Timestamp::now().usec() << endl;
}

int main()
{
	Thread thread(stop);
	thread.start();
	cout << "main thread, threadId = " << CurrThread::tid() << endl;
	eventHandler.setInCallback(true, bind(handler, eventHandler.getHandle()));
	reactor.addEvent(eventHandler);
	cout << "now: " << Timestamp::now().usec() << endl;
	reactor.runAt(timerHandler1, Timestamp::now() + 500000);
	reactor.runAfter(timerHandler2, 1.2);
	reactor.runEvery(timerHandler3, 1.5);
	reactor.run();
	return 0;
}


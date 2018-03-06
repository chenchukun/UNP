#include "Timestamp.h"
#include "Signal.h"
#include "Date.h"
#include "SockAddr.h"
#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"
#include "ThreadManager.h"
#include "GetOpt.h"
#include "BlockingQueue.h"
#include "BoundedBlockingQueue.h"
#include "Singleton.h"
#include <functional>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <queue>
using namespace std;
using namespace snf;

void testTimestamp()
{
	cout << "---------------testTimestamp----------------" << endl;
	Timestamp timestamp;
	cout << timestamp.sec() << endl;
	cout << timestamp.usec() << endl;
	cout << timestamp.msec() << endl;

	Timestamp timestamp2 = Timestamp::now();
	cout << (timestamp == timestamp2) << endl;
	cout << (timestamp > timestamp2) << endl;
	cout << (timestamp >= timestamp2) << endl;
	cout << (timestamp < timestamp2) << endl;
	cout << (timestamp <= timestamp2) << endl;
	Timestamp t = timestamp + timestamp2;
	cout << t.sec() << endl;
	cout << endl;
}

void testDate()
{
	cout << "---------------testDate----------------" << endl;
//	Date date;
//	Date date(Timestamp::now());
	Date date("2017-03-09 14:18:40", "%Y-%m-%d %H:%M:%S");
//	Date date("2017-03-09 14:18", "%Y-%m-%d %H:%M");
	cout << date.format() << endl;
	date.update(Timestamp::now());

//	date.setFormat("%D");

	cout << date.format() << endl;

	cout << date.format("%Y-%m-%d %H:%M") << endl;

	cout << date.getYear() << endl;
	cout << date.getMonth() << endl;
	cout << date.getDay() << endl;
	cout << date.getHour() << endl;
	cout << date.getMinute() << endl;
	cout << date.getSecond() << endl;
	cout << Timestamp::now().sec() << endl;
	cout << date.getTimestamp().sec() << endl;

	Date date2(date);
	cout << date2.format() << endl;
	Date date3;
	date3 = date2;
	cout << date3.format() << endl;
	cout << (date2 == date3) << endl;
	Date date4("2017-03-08 18:06:18", "%Y-%m-%d %H:%M:%S");
	cout << (date4 != date3) << endl;
	cout << (date4 > date3) << endl;
	cout << (date4 >= date3) << endl;
	cout << (date4 < date3) << endl;
	cout << (date4 <= date3) << endl;


	cout << endl;
}

void testSockAddr()
{
	cout << "---------------testSockAddr----------------" << endl;
	SockAddr sockAddr("127.0.0.1", 8888);
	cout << sockAddr.getFamily() << endl;
	cout << sockAddr.getIp() << endl;
	cout << sockAddr.getPort() << endl;

	int ret = 0;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	ret = connect(sockfd, sockAddr.getAddr(), sockAddr.getAddrLength());
	cout << errno << endl;
	assert(ret == 0);

	SockAddr sockAddr2;
	sockAddr2.setFamily(AF_INET);
	sockAddr2.setIp("127.0.0.1");
	sockAddr2.setPort(8888);
	int sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
	ret = connect(sockfd2, sockAddr2.getAddr(), sockAddr2.getAddrLength());
	cout << errno << endl;
	assert(ret == 0);

	SockAddr sockAddr3(sockAddr2.getAddr(), sockAddr2.getAddrLength());
	int sockfd3 = socket(AF_INET, SOCK_STREAM, 0);
	ret = connect(sockfd3, sockAddr3.getAddr(), sockAddr3.getAddrLength());
	cout << errno << endl;
	assert(ret == 0);

	cout << sockAddr.format() << endl;
	cout << sockAddr2.format() << endl;
	cout << sockAddr3.format() << endl;

	SockAddr sockAddr4;
	sockAddr4.setFamily(AF_INET);
	sockAddr4.setAny();
	cout << sockAddr4.getIp() << endl;

	SockAddr sockAddr5;
	sockAddr5.setFamily(AF_INET6);
	sockAddr5.setAny();
	cout << sockAddr5.getIp() << endl;
}

void testGetOpt(int argc, char **argv)
{
	cout << "---------------testGetOpt----------------" << endl;
	GetOpt getOpt(argc, argv, "a:b:c", 1);
	getOpt.setLongOpt("test", 't', GetOpt::OPTIONAL_ARG);
	getOpt.setLongOpt("opt", 'o', GetOpt::OPTIONAL_ARG);
	int ret = getOpt.parse();
	assert(ret == 0);
	int opt;
	while ( (opt=getOpt.getNextOpt()) != -1 ) {
		switch (opt) {
			case 'a':
				cout << "option a:" << getOpt.getOptArg() << endl;
				break;
			case 'b':
				cout << "option b:" << getOpt.getOptArg() << endl;
				break;
			case 'c':
				cout << "option c" << endl;
				break;
			case 't':
				cout << "option t:" << getOpt.getOptArg() << endl;
				break;
			case 'o':
				cout << "option o:" << getOpt.getOptArg() << endl;
				break;
		}
	}
}

void testSignal() 
{
	cout << "---------------testSignal----------------" << endl;
	Signal sig;
	sig.set(SIGINT, [](int sig){cout << sig << " arrive" << endl;});
	while (true) {
		sleep(1);
		sleep(1);
		sleep(1);
		sleep(1);
		assert(sig.defaultAll()==0);
	}
}

Mutex mutex;
int count = 0;

void threadOne()
{
	mutex.lock();
	cout << "threadOne: " << CurrThread::tid() << endl;
	mutex.unlock();
	for (int i=0; i<5000000; ++i) {
		{
			MutexLockGuard lockGuard(mutex);
			++count;
		}
	}
}

void threadTwo() 
{
	mutex.lock();
	cout << "threadTwo: " << CurrThread::tid() << endl;
	mutex.unlock();
	for (int i=0; i<5000000; ++i) {
		{
			MutexLockGuard lockGuard(mutex);
			++count;
		}
	}
}

Condition cond(mutex);
queue<int> que;


void produce()
{
	for (int i=0; i<10; ++i) {
		mutex.lock();
		que.push(i);
		cout << "produce" << endl;
		mutex.unlock();
		cond.notify();
	}
}

void consumer()
{
	while (true) {
		mutex.lock();
		while (que.empty()) {
			cond.wait();
		}
		int x = que.front();
		que.pop();
		mutex.unlock();
		cout << "consumer: " << x << endl;
	}
}

void testThread() 
{
	cout << "---------------testThread----------------" << endl;
//	{
//		MutexLockGuard lockGuard(mutex);
//	}
	Thread thread1(threadOne);
	Thread thread2(threadTwo);
	thread1.start();
	thread2.start();
	thread1.join();
	thread2.join();
	cout << count << endl;
	Thread p(produce);
	Thread c(consumer);
	p.start();
	c.start();
	p.join();
	c.join();
}

void testThreadManager() 
{
	cout << "---------------testThreadManager----------------" << endl;

	ThreadManager threadManager;
	threadManager.create([](){cout << "thread1" << endl;}, 3);
	threadManager.create([](){cout << "thread2" << endl;});
	threadManager.create([](){cout << "thread3" << endl;});
	cout << threadManager.getCount() << endl;
	threadManager.waitAll();
}

BlockingQueue<int> bque;
void t1() 
{
	for (int i=0; i<10; ++i) {
		bque.put(i);
		cout << "put" << endl;
	}
}

void t2()
{
	while (true) {
		int x;
		if (!bque.poll(x, 1.0)) {
			cout << "timeout" << endl;
			continue;
		}
		cout << x << "  " << bque.size() << endl;
	}
}

void testBlockingQueue()
{
	cout << "---------------testBlockingQueue----------------" << endl;
	Thread thread1(t1);
	thread1.start();
	Thread thread2(t2);
	thread2.start();
	thread1.join();
	thread2.join();
}

void bounded1(BoundedBlockingQueue<int> &que)
{
	for (int i=0; i<10; ++i) {
		que.offer(i, 0.5);
		cout << "put " << i << endl;
	}
}

void bounded2(BoundedBlockingQueue<int> &que)
{
	while (true) {
		cout << "size " << que.size() << endl;
		cout << "take " << que.take() << endl;
		sleep(1);
	}
}

void testBoundedBlockingQueue()
{
	cout << "---------------testBoundedBlockingQueue----------------" << endl;
	BoundedBlockingQueue<int> que(3);
	Thread t1(bind(bounded1, ref(que)));
	Thread t2(bind(bounded2, ref(que)));
	t1.start();
	t2.start();
	t1.join();
	t2.join();
}

class SingletonTest
{
public:
	SingletonTest() {
		cout << "SingletonTest" << endl;
	}

	~SingletonTest() {
		cout << "~SingletonTest" << endl;
	}

	void test() {
		cout << "test" << endl;
	}
};

void testSingleton()
{
	cout << "---------------testSingleton----------------" << endl;
	Singleton<SingletonTest>::instance().test();
	Singleton<SingletonTest>::instance().test();
	Singleton<SingletonTest>::destroy();
}

int main(int argc, char **argv)
{
//	testTimestamp();
//	testDate();
//	testSockAddr();
//	testGetOpt(argc, argv);
//	testSignal();
//	testThread();
//	testThreadManager();
//	testBlockingQueue();
//	testBoundedBlockingQueue();
	testSingleton();
	return 0;
}

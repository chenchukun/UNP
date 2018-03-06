#include "ThreadPool.h"
#include "Mutex.h"
#include <iostream>
using namespace std;
using namespace snf;

Mutex mutex;
int count = 0;

void run() 
{
	for (int i=0; i<2500000; ++i) {
		MutexLockGuard lockGuard(mutex);
		++count;
	}
}

int main()
{
	ThreadPool pool(4);
	pool.start();
	pool.run(run);
	pool.run(run);
	pool.run(run);
	pool.run(run);
	cout << pool.getBusyCount() << endl;
	pool.stop();
	cout << count << endl;
	return 0;
}

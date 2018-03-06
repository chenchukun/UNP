#include "Thread.h"
#include <iostream>
using namespace std;
using namespace snf;

void run() 
{
	while (true) {
		cout << "run" << endl;
		sleep(1);
	}
}

void start()
{
	Thread thread(run);
	thread.start();
}

int main()
{
	start();	
	getchar();
	return 0;
}

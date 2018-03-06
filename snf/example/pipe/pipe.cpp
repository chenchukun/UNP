#include "Pipe.h"
#include "Thread.h"
#include <cstring>
#include <iostream>
using namespace std;
using namespace snf;

Pipe p;

void readT() 
{
	char buff[1024];
	int len;
	while ((len = p.read(buff, sizeof(buff))) >=0) {
		buff[len] = 0;
		cout << "read " << buff << endl;
		sleep(2);
	}
}

void writeT() 
{
	char buff[1024] = "Hello World";
	int len;
	while ((len = p.write(buff, strlen(buff))) >=0) {
		cout << "write " << buff << endl;
		sleep(1);
	}
}

int main()
{
	Thread readThread(readT);
	Thread writeThread(writeT);
	readThread.start();
	writeThread.start();
	readThread.join();
	writeThread.join();
}

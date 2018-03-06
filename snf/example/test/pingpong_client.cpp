#include "GetOpt.h"
#include "SockConnect.h"
#include "Condition.h"
#include "Mutex.h"
#include "ThreadManager.h"
#include "Base.h"
#include <iostream>
#include <atomic>
using namespace std;
using namespace snf;

std::string ip = "127.0.0.1";
int port = 6180;
int threadNum = 1;
int runSec = 10;
atomic_int blockNum(0);
int blockSize = 64;
bool run = true;
Mutex mutex;
Condition cond(mutex);


void pingpong() 
{
	SockAddr addr(ip, port);
	SockConnect sockConnect;
	SockStream sockStream;
	int ret = sockConnect.connect(sockStream, addr);
	assert(ret == 0);
	char block[blockSize*1024];
	cond.wait();
	cout << "start run" << endl;
	while (run) {
		int ret = sockStream.write(block, blockSize);
		if (ret != -1) {
			sockStream.read(block, ret);
		}
		++blockNum;
	}
	cout << "run end" << endl;
	sockStream.close();
}

void total() 
{
	cout << "run time: " << runSec << " s" << endl;
	cout << "block size: " << blockSize << "K" << endl;
	cout << "send block number: " << blockNum << endl;
	cout << "throughput: " << static_cast<double>((blockSize * blockNum)) / runSec / 1024 << " MiB/s" << endl;
}


int main(int argc, char **argv)
{
	GetOpt getOpt(argc, argv, "h:p:t:s:b:n:");
	getOpt.parse();
	char opt;
	while ( (opt=getOpt.getNextOpt()) != -1) {
		switch (opt) {
			case 'h':
				ip = getOpt.getOptArg();
				break;
			case 'p':
				port = stoi(getOpt.getOptArg());
				break;
			case 't':
				threadNum = stoi(getOpt.getOptArg());
				break;
			case 's':
				runSec = stoi(getOpt.getOptArg());
				break;
			case 'n':
				blockNum = stoi(getOpt.getOptArg());
				break;
			case 'b':
				blockSize = stoi(getOpt.getOptArg());
				break;
		}
	}
	ThreadManager threadManager;
	threadManager.create(pingpong, threadNum);
	cout << "pingpong client" << endl;
	sleep(5);
	cond.notifyAll();
	sleep(runSec);
	run = false;
	threadManager.waitAll();
	total();
}



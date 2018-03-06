#include "SockHandle.h"
#include <time.h>
#include <assert.h>
#include <iostream>
using namespace snf;
using namespace std;

int main()
{
	SockHandle sockHandle;
	assert(sockHandle.open(AF_INET, SOCK_STREAM)==0);
	int val;
	assert(sockHandle.getOption(SOL_SOCKET, SO_RCVBUF, val)==0);
	cout << val << endl;
	assert(sockHandle.setOption(SOL_SOCKET, SO_RCVBUF, val)==0); // 至少是MSS值的4倍, 设置的真实值是val的两倍？
	assert(sockHandle.getOption(SOL_SOCKET, SO_RCVBUF, val)==0);
	cout << val << endl;
	struct timeval tval = {1, 1};
	assert(sockHandle.getOption(SOL_SOCKET, SO_RCVTIMEO, tval)==0);
	cout << tval.tv_sec << "." << tval.tv_usec<< endl;
	tval.tv_sec = tval.tv_usec = 100;
	assert(sockHandle.setOption(SOL_SOCKET, SO_RCVTIMEO, tval)==0);
	assert(sockHandle.getOption(SOL_SOCKET, SO_RCVTIMEO, tval)==0);
	cout << tval.tv_sec << "." << tval.tv_usec<< endl;

	return 0;
}

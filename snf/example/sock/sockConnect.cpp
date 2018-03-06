#include "SockConnect.h"
#include <cstring>
#include <iostream>
using namespace std;
using namespace snf;

void echo(SockStream &sockStream)
{
	char buff[1024];
	while (cin >> buff) {
		sockStream.write(buff, strlen(buff));
		int len = sockStream.read(buff, sizeof(buff));
		buff[len] = 0;
		cout << buff << endl;
	}
}

int main()
{
	SockStream sockStream;
	SockConnect sockConnect;
	SockAddr remoteAddr("127.0.0.1", 6180);
	int ret = sockConnect.connect(sockStream, remoteAddr, 1.2);
	if (ret != 0) {
		cout << ret << endl;
		return 0;
	}
	cout << "connect success" << endl;
	echo(sockStream);
	return 0;
}

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <arpa/inet.h>
#include <assert.h>
#include <cstring>
#include <errno.h>
#include <unistd.h>
using namespace std;

// 网络字节序和主机字节序的转换
void testHton()
{
	cout << "------------------testHton----------------" << endl;
	uint16_t host16 = 618;
	uint32_t host32 = 6180;
	uint16_t net16 = htons(host16);
	uint32_t net32 = htonl(host32);
	cout << "host16: " << host16 << endl;
	cout << "host32: " << host32 << endl;
	cout << "hton:" << endl;
	cout << "net16: " << net16 << endl;
	cout << "net32: " << net32 << endl;
	cout << "ntoh:" << endl;
	cout << ntohs(net16) << endl;
	cout << ntohl(net32) << endl;
	cout << endl;
}


// 字符串ip和二进制ip地址的转换
void testPton()
{
	cout << "------------------testPton----------------" << endl;
	const char *ipv4 = "192.168.1.1";
	const char *ipv6 = "2001:da8:2019:b228:29a:9bff:fe96:78b8";
	int ret = 0;
	uint32_t ipv4t = 0;
	struct in_addr ipv4i;
	ret = inet_pton(AF_INET, ipv4, &ipv4t);
	assert(ret == 1);
	cout << ipv4t << endl;
	ret = inet_pton(AF_INET, ipv4, &ipv4i);
	cout << ipv4i.s_addr << endl;

	struct in6_addr ipv6i;
	ret = inet_pton(AF_INET6, ipv6, &ipv6i);
	assert(ret == 1);

	char ipv4c[16];
	char ipv6c[64];
	const char *retc = inet_ntop(AF_INET, &ipv4i, ipv4c, sizeof(ipv4c));
	assert(retc != NULL);
	cout << retc << endl;

	retc = inet_ntop(AF_INET6, &ipv6i, ipv6c, sizeof(ipv6c));
	assert(retc != NULL);
	cout << retc << endl;
	cout << endl;
}


void testAddr()
{
	cout << "------------------testAddr----------------" << endl;
	struct sockaddr_in sockaddrv4;
	struct sockaddr_in6 sockaddrv6;
	struct sockaddr sockaddr;
	struct sockaddr_storage sockaddrs;

	cout << sizeof(sockaddrv4) << endl;
	cout << sizeof(sockaddrv6) << endl;
	cout << sizeof(sockaddr) << endl;
	cout << sizeof(sockaddrs) << endl;
}


void testSocketServer()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(sockfd != -1);
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = INADDR_ANY;
	int ret = bind(sockfd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr));
	assert(ret == 0);
	ret = listen(sockfd, 128);
	assert(ret == 0);
	while(true) {
		struct sockaddr_in remoteAddr;
		socklen_t socklen = sizeof(remoteAddr);
		int remoteSockfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&remoteAddr), &socklen);
		if (remoteSockfd == -1) {
			cout << "accept error" << endl;
			continue;
		}
		cout << "connect success." << endl;
		shutdown(remoteSockfd, SHUT_RD);
		/*
		char name[64];
		int len = read(remoteSockfd, name, 64);
		if (len <= 0) {
			close(remoteSockfd);
			continue;
		}
		name[len] = 0;
		string sentStr = "hello ";
		sentStr += name;
		sentStr += "\n";
		write(remoteSockfd, sentStr.c_str(), sentStr.size());
		close(remoteSockfd);
		*/
	}
}

void testSocketClient()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(sockfd != -1);
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	int ret = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	assert(ret == 1);
	ret = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	assert(ret == 0);
	cout << "connect success." << endl;
	char buff[128];
	while (cin >> buff) {
		int n = write(sockfd, buff, strlen(buff));
		cout << n << endl;
	}

	/*
	write(sockfd, "cck", 3);
	int len = read(sockfd, buff, 128);
	if (len <= 0) {
		close(sockfd);
		return;
	}
	buff[len] = 0;
	cout << buff << endl;
	close(sockfd);
	*/
}


int main()
{
	testHton();
	testPton();
	testAddr();
	testSocketServer();
//	testSocketClient();
	return 0;
}


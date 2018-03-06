#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
using namespace std;

void testGethostbyname(const char *hostname)
{
	char ip[128];
	struct hostent *host = gethostbyname(hostname);
	if (host == NULL) {
		cout << hstrerror(h_errno) << endl;
		return;
	}
	cout << "h_name: " << host->h_name << endl;  // 主机名
	char **h_aliases = host->h_aliases;	
	while (*h_aliases != NULL) {				 // 别名
		cout << "h_aliases: " << *h_aliases++ << endl;
	}
	cout << "h_addrtype: " << host->h_addrtype << endl; // 地址类型 AF_INET 或 AF_INET6
	cout << "h_length: " << host->h_length << endl;		// 地址长度
	char **h_addr_list = host->h_addr_list;
	while (*h_addr_list != NULL) {
		cout << "h_addr_list: " << inet_ntop(host->h_addrtype, *h_addr_list++, ip, sizeof(ip)) << endl;
	}
}

void testGethostbyaddr(const char *ip)
{
	struct in_addr addr;
	inet_pton(AF_INET, ip, &addr);
	struct hostent *host = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	if (host == NULL) {
		cout << hstrerror(h_errno) << endl;
		return;
	}
	cout << "h_name: " << host->h_name << endl;  // 主机名
	char **h_aliases = host->h_aliases;	
	while (*h_aliases != NULL) {				 // 别名
		cout << "h_aliases: " << *h_aliases++ << endl;
	}
	cout << "h_addrtype: " << host->h_addrtype << endl; // 地址类型 AF_INET 或 AF_INET6
	cout << "h_length: " << host->h_length << endl;		// 地址长度
	char **h_addr_list = host->h_addr_list;
	char sip[128];
	while (*h_addr_list != NULL) {
//		cout << "h_addr_list: " << *h_addr_list++ << endl;
		cout << "h_addr_list: " << inet_ntop(host->h_addrtype, *h_addr_list++, sip, sizeof(sip)) << endl;
	}
}

int main(int argc, char **argv)
{
//	testGethostbyname(argv[1]);
	testGethostbyaddr(argv[1]);
	return 0;
}

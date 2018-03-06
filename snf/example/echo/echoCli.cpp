#include <iostream>
#include <poll.h>
#include <assert.h>
#include <string>
#include <unistd.h>
#include "SockAddr.h"
#include "GetOpt.h"
using namespace std;
using namespace snf;

class EchoClient
{
public:
	EchoClient(string ip, int port):ip(ip), port(port) {}

	void start();

private:
	string ip;
	int port;
};

void EchoClient::start() 
{
	SockAddr addr(ip, port);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);
	int ret = connect(sock, addr.getAddr(), addr.getAddrLength());
	assert(ret!=-1);
	struct pollfd fdarray[2];
	fdarray[0] = {0, POLLIN, 0};
	fdarray[1] = {sock, POLLIN, 0};
	char buff[1024];
	
	while (true) {
		ret = poll(fdarray, 2, -1);
		if (ret == -1 && errno==EINTR) {
			continue;
		}
		if (fdarray[0].revents | POLLIN) {
			int n = read(0, buff, 1024);
			if (n > 0) {
				write(sock, buff, n);
			}
		}
		if (fdarray[1].revents | POLLIN) {
			int n = read(sock, buff, 1024);
			if (n > 0) {
				write(1, buff, n);
			}
		}
	}
}

int main(int argc, char **argv) 
{
	GetOpt getopt(argc, argv, "b:p:");
	int ret = getopt.parse();
	assert(ret == 0);
	int opt;
	string ip = "127.0.0.1";
	int port = 6180;
	while ((opt=getopt.getNextOpt()) != -1) {
		switch(opt) {
			case 'b':
				ip = getopt.getOptArg();
				break;
			case 'p':
				port = stoi(getopt.getOptArg());
				break;
		}
	}
	EchoClient client(ip, port);
	client.start();
	return 0;
}

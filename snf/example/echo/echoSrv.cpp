#include "SockAddr.h"
#include "GetOpt.h"
#include "Singleton.h"
#include "Signal.h"
#include <sys/wait.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;
using namespace snf;

class EchoSrv
{
public:
	EchoSrv(string ip, int port):ip(ip), port(port) {}
	void start();
	void echo(int fd);
//	SockAddr sockaddr("127.0.0.1", 6180);
private:
	string ip;
	int port;
};

void EchoSrv::start() 
{
	SockAddr sockaddr(ip, port);
	int lisfd = socket(AF_INET, SOCK_STREAM, 0);
	int ret = bind(lisfd, sockaddr.getAddr(), sockaddr.getAddrLength());
	assert(ret == 0);
	ret = listen(lisfd, 10);
	assert(ret == 0);
	while (true) {
		struct sockaddr_storage peerAddr;
		socklen_t len = sizeof(peerAddr);
		int sock = accept(lisfd, reinterpret_cast<struct sockaddr*>(&peerAddr), &len);
		// ECONNABORTED 处理客户端connect后立即发送RST给服务器的情况
		if (sock == -1 && (errno==EINTR || errno==ECONNABORTED)) {
			continue;
		}
		assert(sock >= 0);
		SockAddr addr(reinterpret_cast<struct sockaddr*>(&peerAddr), len);
		cout << addr.format() << " content" << endl;
		pid_t pid = fork();
		if (pid == 0) {
			close(lisfd);
			echo(sock);
			close(sock);
			exit(0);
		}
		close(sock);
	}

}

void EchoSrv::echo(int fd)
{
	int n = 0;
	const int BUFF_SIZE = 100;
	char buff[BUFF_SIZE];
	while (true) {
		n = read(fd, buff, BUFF_SIZE);
		if ( n == -1) {
			if (errno == EINTR) {
				continue;
			}
			else {
				break;
			}
		}
		else if(n == 0) {
			break;
		}
		write(fd, buff, n);
	}
}

void sigChld(int sig)
{
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
		cout << "child " << pid << " terminated" << endl;
	}
}

int main(int argc, char **argv) 
{
	Singleton<Signal>::instance().set(SIGCHLD, sigChld);
	GetOpt getopt(argc, argv, "b:p:");
	int ret = getopt.parse();
	assert(ret == 0);
	int opt;
	string ip = "0.0.0.0";
	int port = 6180;
	while ((opt = getopt.getNextOpt()) != -1) {
		switch(opt) {
		case 'b':
			ip = getopt.getOptArg();
			break;
		case 'p':
			port = stoi(getopt.getOptArg());
			break;
		}
	}
	EchoSrv echoSrv(ip, port);
	echoSrv.start();
	return 0;
}

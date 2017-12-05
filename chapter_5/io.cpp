//
// Created by chenchukun on 17/12/3.
//

#include <iostream>
#include <string>
#include <getopt.h>
#include <sys/socket.h>
#include <memory>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <assert.h>
#include "sock.h"
#include <signal.h>
#include <sys/wait.h>
#include "utility.h"
using namespace std;

//#define TEST_WRITE
#define TEST_READ

void sigPipe(int signo)
{
    cout << "SIGPIPE" << endl;
}

int main()
{
    int lisfd = sock_listen(AF_INET, 10, 6180);
    assert(lisfd >= 0);
    signal_action(SIGPIPE, sigPipe);
    while (true) {
        int connfd = accept(lisfd, NULL, NULL);
        assert(connfd >= 0);
        char buff[128];
        ssize_t  n = read(connfd, buff, sizeof(128));
        assert(n >= 0);
        write(connfd, buff, n);
#ifdef TEST_WRITE
        // close(connfd);
        // shutdown(connfd, SHUT_RDWR);
        // shutdown(connfd, SHUT_RD);
        // 调用close()主动关闭连接后,write(),产生EBADF错误?
        // 调用shutdown()关闭写后,write(),产生PIPE错误?
        // 调用shutdown()关闭读,write()正常
        write(connfd, buff, n);
        cout << strerror(errno) << "(" << errno << ")" << endl;
#endif
#ifdef TEST_READ
        cout << "1" << endl;
        // 关闭写,称为半关闭,缓冲区中的数据将被发送,后发送FIN,不可再调用写函数
//        shutdown(connfd, SHUT_WR);
        // 关闭读,不发送FIN,表示不再接受数据,且在缓冲区中的数据会被丢弃
//        shutdown(connfd, SHUT_RD);
        close(connfd);
        cout << "1" << endl;
        // 关闭写,可以正常读
        // 关闭读,一直阻塞,有数据达到也不返回?
        // 调用close(),产生EBADF错误?
        n = read(connfd, buff, sizeof(buff));
        cout << "1" << endl;
        if (n < 0) {
            cout << strerror(errno) << "(" << errno << ")" << endl;
        }
        else {
            write(STDOUT_FILENO, buff, n);
        }
#endif
        cout << "finish" << endl;
    }
    return 0;
}
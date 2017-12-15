//
// Created by chenchukun on 17/12/15.
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
#include <thread>
#include "sock.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <assert.h>
#include <set>
#include <fcntl.h>
#include "utility.h"
using namespace std;

/*
 * fcntl在网络编程中最常用的操作是用来将一个套接字设置为非阻塞I/O
 * 若成功返回值取决于cmd,失败返回-1
 */

int nonblock(int connfd, int op)
{
    int flags = fcntl(connfd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    if (op) {
        flags |= O_NONBLOCK;
    }
    else {
        flags &= ~O_NONBLOCK;
    }
    return fcntl(connfd, F_SETFL, flags);
}

int main(int argc, char **argv)
{
    int connfd = sock_connect(AF_INET, stoi(argv[2]), argv[1]);
    assert(connfd >= 0);
    nonblock(connfd, true);
    char buff[100];
    read(connfd, buff, sizeof(buff));
    PrintStdError("read返回:");
    nonblock(connfd, false);
    read(connfd, buff, sizeof(buff));
    PrintStdError("read返回:");
    close(connfd);
    return 0;
}
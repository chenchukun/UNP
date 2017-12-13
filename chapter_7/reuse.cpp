//
// Created by chenchukun on 17/12/13.
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
#include "utility.h"
using namespace std;

int main(int argc, char **argv)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    assert(ret == 0);
    // SO_REUSEADDR允许在同一端口上启动同一服务的多个实例,只要每个实例捆绑一个不同的本地IP地址即可
    // Linux下不允许将端口绑定在统配地址上后再绑定其他IP地址
    assert(sock_bind(listenfd, AF_INET, stoi(argv[2]), argv[1])==0);
    assert(listen(listenfd, 10) == 0);
    while (true) {
        int connfd = accept(listenfd, NULL, NULL);
        if (connfd >= 0) {
            write(connfd, "hello world\n", 12);
            close(connfd);
        }
    }
    close(listenfd);
    return 0;
}
//
// Created by chenchukun on 17/12/7.
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

int main()
{
    int listenfd = sock_listen(AF_INET, 10, 6180);
    if (listenfd < 0) {
        PrintStdError("创建监听套接字失败");
        exit(1);
    }
    int optval;
    socklen_t len = sizeof(optval);
    // getsockopt用于获取套接字选项的值,成功返回0,失败返回-1
    // optval用于返回套接字选项的值,根据选项类型,可以是一个整数,也可以是一个结构
    // len为值-结果参数
    int ret = getsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &optval, &len);
    assert(ret == 0);
    cout << "SO_RCVBUF = " << optval << endl;

    optval = 425984;
    ret = setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &optval, len);
    assert(ret == 0);
    // 设置套接字选项的值
    ret = getsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &optval, &len);
    assert(ret == 0);
    cout << "SO_RCVBUF = " << optval << endl;
    close(listenfd);
    return 0;
}
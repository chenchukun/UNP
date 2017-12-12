//
// Created by chenchukun on 17/12/11.
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

// /proc/sys/net/ipv4/tcp_wmem 记录TCP发送缓冲区的最小值、默认值和最大值
// /proc/sys/net/ipv4/tcp_rmem 记录TCP接收缓冲区的最小值、默认值和最大值

// /proc/sys/net/core/wmem_max   保存发送缓冲区可设置的最大值的一半,即setsockopt可接收的最大值
// /proc/sys/net/core/rmem_max   保存接收缓冲区可设置的最大值的一半,即setsockopt可接收的最大值

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int optval;
    int ret;
    socklen_t len = sizeof(optval);
    // 获取默认缓冲区大小
    ret = getsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &optval, &len);
    assert(ret == 0);
    cout << "DEFAULT SO_RCVBUF = " << optval << endl;
    ret = getsockopt(listenfd, SOL_SOCKET, SO_SNDBUF, &optval, &len);
    assert(ret == 0);
    cout << "DEFAULT SO_SNDBUF = " << optval << endl;
    // 对于监听套接字缓冲区大小必须在调用listen前设置才有效,
    // 调用listen后开始接收客户端链接,已连接套接字从监听套接字继承该值,所有必须在调用listen之前设置
    optval = 40960;
    // 通过setsockopt设置缓冲区大小,若optval小于对于/proc/sys/net/core/*mem_max
    // 则设置为optval*2,否则设置为*mem_max*2
    ret = setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &optval, len);
    assert(ret == 0);
    optval = 40960;
    ret = setsockopt(listenfd, SOL_SOCKET, SO_SNDBUF, &optval, len);
    assert(ret == 0);
    if (sock_listen(listenfd, AF_INET, 10, 6180) == -1) {
        PrintStdError("创建监听套接字失败");
        exit(1);
    }
    len = sizeof(optval);
    ret = getsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &optval, &len);
    assert(ret == 0);
    cout << "SO_RCVBUF = " << optval << endl;
    ret = getsockopt(listenfd, SOL_SOCKET, SO_SNDBUF, &optval, &len);
    assert(ret == 0);
    cout << "SO_SNDBUF = " << optval << endl;

    // 接收低水位标记:默认值为1
    ret = getsockopt(listenfd, SOL_SOCKET, SO_RCVLOWAT, &optval, &len);
    assert(ret == 0);
    cout << "DEFAULT SO_RCVLOWAT = " << optval << endl;
    // 发送低水位标记:默认值为1
    ret = getsockopt(listenfd, SOL_SOCKET, SO_SNDLOWAT, &optval, &len);
    assert(ret == 0);
    cout << "DEFAULT SO_SNDLOWAT = " << optval << endl;
    return 0;
}
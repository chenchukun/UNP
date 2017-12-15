//
// Created by chenchukun on 17/12/14.
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
#include <netinet/tcp.h>
#include "utility.h"
using namespace std;

/*
 * Nagle算法的目的在于减少广域网(WAN)上小分组(小于MSS的分组)的数目,从而减小网络拥塞的出现
 * Nagle算法要求一个TCP连接上同一时刻最多只有一个未被确认的小分组,多个小分组将在ACK到达后组成一个分组发送
 * Nagle算法通常与ACK延滞算法一起使用
 */
int main()
{
    int connfd = sock_connect(AF_INET, 6180, "127.0.0.1");
    assert(connfd >= 0);
    int optval = 1;
    int ret = setsockopt(connfd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
    assert(ret == 0);
    // 此时让服务端主机崩溃
    cin >> optval;
    // 抓包观察分组发送情况
    write(connfd, "1", 1);
    Sleep(1000000);
    write(connfd, "2", 1);
    Sleep(1000000);
    write(connfd, "3", 1);
    return 0;
}
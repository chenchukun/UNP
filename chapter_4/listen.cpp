//
// Created by chenchukun on 17/11/29.
//

#include <sys/socket.h>
#include <iostream>
#include <memory>
#include <assert.h>
#include <vector>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <thread>
#include "sock.h"
using namespace std;

void con()
{
    int count = 0;
    vector<int> vecSock;
    while (true) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        assert(sockfd >= 0);
        sockaddr_in addr;
        assert(sockaddr_set(&addr, AF_INET, 6180, "127.0.0.1") == 0);
        assert(connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == 0);
        vecSock.push_back(sockfd);
        cout << "客户端" << ++count << "\t连接成功, sockfd = " << sockfd << endl;
    }
}

int main(int argc, char **argv)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    shared_ptr<int> raii(&sockfd, [](int *p){close(*p);});
    assert(sockfd >= 0);
    assert(sock_bind(sockfd, AF_INET, 6180) == 0);
    int backlog = 10;
    // 通过环境变量获取backlog的值
    const char *p = getenv("LISTENQ");
    if (p != NULL) {
        backlog = stoi(p);
    }
    // 把一个未连接的套接字转换成一个被动套接字,指示内核应接受指向该套接字的连接请求
    // 套接字从CLOSED状态转为LISTEN状态,backlog规定了内核应该为相应套接字排队的最大连接个数
    int ret = listen(sockfd, backlog);
    if (ret == 0) {
        cout << "listen调用成功" << endl;
    }
    else {
        cout << "listen调用失败: " << strerror(errno) << endl;
    }

    /*
     *  内核为每一个监听套接字维护两个队列:
     *  未完成连接队列 -- 保存接收到客户端的SYN,并发出ACK和SYN,等待三次握手完成处于SYNC_RECV状态的套接字
     *  已完成连接队列 -- 保存已完成三次握手过程,处于ESTABLISHED状态的套接字
     *  当一个客户端的SYN达到时,若外完成队列已满,TCP将忽略该SYNC,并不发送RST,客户端会重发SYN,直到超时或队列有空间
     *  对于backlog的定义没有正式的定义,在Linux中,表示已完成连接队列最长为backlog + 1
     *  /proc/sys/net/ipv4/tcp_max_syn_backlog  记录当前backlog允许的最大值
     *  /proc/sys/net/core/somaxconn
     */
    thread conThread(con);
    conThread.join();
    return 0;
}
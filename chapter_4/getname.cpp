//
// Created by chenchukun on 17/12/1.
//

#include <sys/socket.h>
#include <iostream>
#include <memory>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include "sock.h"
using namespace std;

/*
 * getsockname()通过套接字描述符获取与之关联的本地协议地址
 * getpeername()通过套接字描述符获取与之关联的外地协议地址
 */

int main(int argc, char **argv)
{
    int lisfd = sock_listen(AF_INET, 10);
    if (lisfd < 0) {
        cerr << "创建监听套接字失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    // 以端口0调用bind()后,内核分配一个临时的端口后,通过getsockname()可以获取到
    if (getsockname(lisfd, reinterpret_cast<sockaddr*>(&addr), &len) == -1) {
        cerr << "调用sgetsockname失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    string host;
    if (sock_ntop(&addr, host) == NULL) {
        cerr << "调用sock_ntop失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    cout << "监听端口:" << host << endl;

    int sockfd = accept(lisfd, NULL, NULL);
    assert(sockfd >= 0);

    // 以统配地址调用bind(),接收连接后,内核根据数据入口选择IP,调用getsockname()可以获取到
    if (getsockname(sockfd, reinterpret_cast<sockaddr*>(&addr), &len) == -1) {
        cerr << "调用sgetsockname失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    if (sock_ntop(&addr, host) == NULL) {
        cerr << "调用sock_ntop失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    cout << "接收连接IP:" << host << endl;

    // 调用getpeername()可以获取到套接字描述符管理的远程地址
    if (getpeername(sockfd, reinterpret_cast<sockaddr*>(&addr), &len) == -1) {
        cerr << "调用sgetpeername失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    if (sock_ntop(&addr, host) == NULL) {
        cerr << "调用sock_ntop失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    cout << "客户端地址:" << host << endl;
    int family = sockfd_to_family(sockfd);
    cout << "协议类型:" << family_to_name(family) << endl;
    close(sockfd);
    close(lisfd);

    return 0;
}
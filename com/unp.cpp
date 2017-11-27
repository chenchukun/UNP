//
// Created by chenchukun on 17/11/27.
//
#include "unp.h"

// 将统配地址和一个临时端口绑定到一个套接字上
// 成功返回端口号,失败返回-1
int sock_bind_wild(int sockfd, int family)
{
    socklen_t len;
    switch (family) {
        case AF_INET: {
            sockaddr_in addr;
            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = 0;
            if (bind(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))<0) {
                return -1;
            }
            len = sizeof(addr);
            if (getsockname(sockfd, reinterpret_cast<sockaddr*>(&addr), &len) < 0) {
                return -1;
            }
            return ntohs(addr.sin_port);
        }
        case AF_INET6: {
            sockaddr_in6 addr;
            bzero(&addr, sizeof(addr));
            addr.sin6_family = AF_INET6;
            addr.sin6_addr = IN6ADDR_ANY_INIT;
            addr.sin6_port = 0;
            if (bind(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
                return -1;
            }
            len = sizeof(addr);
            if (getsockname(sockfd, reinterpret_cast<sockaddr*>(&addr), &len) < 0) {
                return -1;
            }
            return ntohs(addr.sin6_port);
        }
    }
    return -1;
}
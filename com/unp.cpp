//
// Created by chenchukun on 17/11/27.
//
#include "unp.h"
using namespace std;

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
        default:
            errno = EAFNOSUPPORT;
            return -1;
    }
    return -1;
}

int sock_bind(int sockfd, int family, int16_t port, const char *ip)
{
    switch (family) {
        case AF_INET: {
            sockaddr_in addr;
            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            if (ip == NULL) {
                addr.sin_addr.s_addr = INADDR_ANY;
            }
            else {
                int ret = inet_pton(AF_INET, ip, &addr.sin_addr);
                if (ret != 1) {
                    return -1;
                }
            }
            addr.sin_port = htons(port);
            if (bind(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))<0) {
                return -1;
            }
            break;
        }
        case AF_INET6: {
            sockaddr_in6 addr;
            bzero(&addr, sizeof(addr));
            addr.sin6_family = AF_INET6;
            if (ip == NULL) {
                addr.sin6_addr = IN6ADDR_ANY_INIT;
            }
            else {
                int ret = inet_pton(AF_INET6, ip, &addr.sin6_addr);
                if (ret != 1) {
                    return -1;
                }
            }
            addr.sin6_port = htons(port);
            if (bind(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr))<0) {
                return -1;
            }
            break;
        }
        default:
            errno = EAFNOSUPPORT;
            return -1;
    }
    return 0;
}

int sockaddr_set(void *addr, int family, int16_t port, const char *ip)
{
    switch (family) {
        case AF_INET: {
            sockaddr_in *addr4 = reinterpret_cast<sockaddr_in*>(addr);
            bzero(addr4, sizeof(sockaddr_in));
            addr4->sin_family = family;
            if ( ip == NULL ) {
                addr4->sin_addr.s_addr = INADDR_ANY;
            }
            else {
                if (inet_pton(family, ip, &addr4->sin_addr) != 1) {
                    return -1;
                }
            }
            addr4->sin_port = htons(port);
            break;
        }
        case AF_INET6: {
            sockaddr_in6 *addr6 = reinterpret_cast<sockaddr_in6*>(addr);
            bzero(addr6, sizeof(sockaddr_in6));
            addr6->sin6_family = family;
            if ( ip == NULL ) {
                addr6->sin6_addr = IN6ADDR_ANY_INIT;
            }
            else {
                if (inet_pton(family, ip, &addr6->sin6_addr) != 1) {
                    return -1;
                }
            }
            addr6->sin6_port = htons(port);
            break;
        }
        default:
            return -1;
    }
    return 0;
}

const char* sock_ntop(const void *vaddr, char *host, int len)
{
    const sockaddr *addr = reinterpret_cast<const sockaddr*>(vaddr);
    switch(addr->sa_family) {
        case AF_INET: {
            const sockaddr_in *sockaddrIn = reinterpret_cast<const sockaddr_in *>(addr);
            if (inet_ntop(AF_INET, &sockaddrIn->sin_addr, host, len) == NULL) {
                return NULL;
            }
            uint16_t port = ntohs(sockaddrIn->sin_port);
            if (port != 0) {
                sprintf(host + strlen(host), ":%d", port);
            }
            break;
        }
        case AF_INET6: {
            const sockaddr_in6 *sockaddrIn6 = reinterpret_cast<const sockaddr_in6 *>(addr);
            if (inet_ntop(AF_INET6, &sockaddrIn6->sin6_addr, host, len) == NULL) {
                return NULL;
            }
            uint16_t port = ntohs(sockaddrIn6->sin6_port);
            if (port != 0) {
                sprintf(host + strlen(host), ":%d", port);
            }
            break;
        }
        default:
            return NULL;
    }
    return host;
}

int sock_listen(int family, int backlog, int16_t port, const char *ip)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        close(sockfd);
        return -1;
    }
    if (sock_bind(sockfd, family, port, ip) == -1) {
        close(sockfd);
        return -1;
    }
    if (listen(sockfd, backlog) == -1) {
        close(sockfd);
        return -1;
    }
    return sockfd;
}

//
// Created by chenchukun on 17/11/27.
//
#include "sock.h"
#include <signal.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <iostream>
using namespace std;

int readn(int fd, void *buff, size_t len)
{
    size_t offset = 0;
    int nread = 0;
    char *ptr = (char *) buff;
    while (true) {
        nread = read(fd, ptr+offset, len-offset);
        if (nread == 0) {
            break;
        }
        if (nread > 0) {
            offset += nread;
            continue;
        }
        if (errno == EINTR) {
            continue;
        }
        return -1;
    }
    return offset;
}

int writen(int fd, const void *buff, size_t len)
{
    int nwrite = 0;
    size_t offset = 0;
    const char *ptr = (const char *) buff;
    while (true) {
        nwrite = write(fd, ptr+offset, len-offset);
        // write()什么时候返回0???
        if (nwrite == 0) {
            break;
        }
        if (nwrite > 0) {
            offset += nwrite;
            continue;
        }
        if (errno == EINTR) {
            continue;
        }
        return -1;
    }
    return offset;
}

int readline(int fd, void *buff, size_t len)
{
    size_t nread = len;
    char *ptr = reinterpret_cast<char*>(buff);
    for (size_t i=0; i<len-1; ++i) {
        int ret = read(fd, ptr+i, 1);
        if (ret == 1) {
            if (*(ptr + i) == '\n') {
                if (i - 1 >= 0 && *(ptr + i - 1)=='\r') {
                    *(ptr + i - 1) = 0;
                    return i-1;
                }
                *(ptr + i) = 0;
                return i;
            }
        }
        else if (ret == 0) {
            *(ptr+i) = 0;
            return i;
        }
        else if (errno == EINTR) {
            --i;
            continue;
        }
        return -1;
    }
    *(ptr + len-1) = 0;
    return len - 1;
}

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

int sock_bind(int sockfd, int family, uint16_t port, const char *ip)
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

int sockaddr_set(void *addr, int family, uint16_t port, const char *ip)
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
            errno = EAFNOSUPPORT;
            return -1;
    }
    return 0;
}

int sock_ntop(const void *vaddr, char *ip, int len, uint16_t &port)
{
    const sockaddr *addr = reinterpret_cast<const sockaddr*>(vaddr);
    switch(addr->sa_family) {
        case AF_INET: {
            const sockaddr_in *sockaddrIn = reinterpret_cast<const sockaddr_in *>(addr);
            if (inet_ntop(AF_INET, &sockaddrIn->sin_addr, ip, len) == NULL) {
                return -1;
            }
            port = ntohs(sockaddrIn->sin_port);
            break;
        }
        case AF_INET6: {
            const sockaddr_in6 *sockaddrIn6 = reinterpret_cast<const sockaddr_in6 *>(addr);
            if (inet_ntop(AF_INET6, &sockaddrIn6->sin6_addr, ip, len) == NULL) {
                return -1;
            }
            port = ntohs(sockaddrIn6->sin6_port);
            break;
        }
        default:
            errno = EAFNOSUPPORT;
            return -1;
    }
    return 0;
}

const char* sock_ntop(const void *vaddr, char *host, int len)
{
    uint16_t port;
    if (sock_ntop(vaddr, host, len, port) != 0) {
        return NULL;
    }
    if (port == 0) {
        return host;
    }
    strcat(host, ":");
    strcat(host, to_string(port).c_str());
    return host;
}

const char* sock_ntop(const void *vaddr, std::string &host)
{
    char buff[128];
    if (sock_ntop(vaddr, buff, sizeof(buff)) == NULL) {
        return NULL;
    }
    host = buff;
    return host.c_str();
}

int sock_ntop(const void *vaddr, std::string &ip, uint16_t &port)
{
    char buff[128];
    if (sock_ntop(vaddr, buff, sizeof(buff), port) != 0) {
        return -1;
    }
    ip = buff;
    return 0;
}

int sock_listen(int family, int backlog, uint16_t port, const char *ip)
{
    int sockfd = socket(family, SOCK_STREAM, 0);
    return sock_listen(sockfd, family, backlog, port, ip);
}

int sock_listen(int sockfd, int family, int backlog, uint16_t port, const char *ip)
{
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

int sockfd_to_family(int sockfd)
{
    sockaddr_storage addr;
    socklen_t len = sizeof(addr);
    if (getsockname(sockfd, reinterpret_cast<sockaddr*>(&addr), &len) != 0) {
        return -1;
    }
    return addr.ss_family;
}

int sock_connect(int family, uint16_t port, const char *ip)
{
    int connfd = socket(family, SOCK_STREAM, 0);
    if (connfd < 0) {
        return -1;
    }
    sockaddr_storage addr;
    if (sockaddr_set(&addr, family, port, ip) != 0) {
        close(connfd);
        return -1;
    }
    // MAC不加这个会报错?
    socklen_t len = family==AF_INET?sizeof(sockaddr_in):sizeof(sockaddr_in6);
    if (connect(connfd, reinterpret_cast<sockaddr*>(&addr), len) != 0) {
        close(connfd);
        return -1;
    }
    return connfd;
}

SigFunc* signal_action(int signo, SigFunc *func)
{
    struct sigaction act, oact;
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }
    return oact.sa_handler;
}

struct addrinfo* host_serv(const char *hostname, const char *service, int family, int socktype, int flags)
{
    struct addrinfo *result;
    struct addrinfo hints;
    bzero(&hints, sizeof(hints));
    hints.ai_flags = flags;
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    int ret = getaddrinfo(hostname, service, &hints, &result);
    if (ret != 0) {
        errno = ret;
        return NULL;
    }
    return result;
}

int tcp_connect(const char *hostname, const char *service)
{
    struct addrinfo *result;
    result = host_serv(hostname, service, AF_UNSPEC, SOCK_STREAM);
    if (result == NULL) {
        return -1;
    }
    struct addrinfo *ptr = result;
    int sockfd;
    while (ptr != NULL) {
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sockfd >= 0) {
            if (connect(sockfd, ptr->ai_addr, ptr->ai_addrlen) == 0) {
                break;
            }
            close(sockfd);
        }
        ptr = ptr->ai_next;
    }
    freeaddrinfo(result);
    if (ptr == NULL) {
        return -1;
    }
    return sockfd;
}


int tcp_connect_nonblock(const char *hostname, const char *service, int sec)
{
    struct addrinfo *result = host_serv(hostname, service, AF_UNSPEC, SOCK_STREAM);
    if (result == NULL) {
        return -1;
    }
    struct addrinfo *ptr = result;
    int sockfd;
    uint32_t usec = sec * 1000000;
    timeval timeout;
    int ret;
    bool error = false;
    while (ptr != NULL) {
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (usec > 0) {
            timeout.tv_sec = usec / 1000000;
            timeout.tv_usec = usec % 1000000;
        }
        if (sockfd >= 0) {
            set_nonblock(sockfd, true);
            ret = connect(sockfd, ptr->ai_addr, ptr->ai_addrlen);
            // 连接成功
            if (ret == 0) {
                break;
            }
            // 发送错误
            if (ret == -1 && errno != EINPROGRESS) {
                ptr = ptr->ai_next;
                close(sockfd);
                continue;
            }
            fd_set rset, wset;
            FD_ZERO(&rset);
            FD_SET(sockfd, &rset);
            wset = rset;
            do {
                ret = select(sockfd+1, &rset, &wset, NULL, usec<=0?NULL: &timeout);
            } while (ret == -1 && errno == EINTR);
            // 超时
            if (ret == 0) {
                errno = ETIMEDOUT;
                error = true;
                close(sockfd);
                break;
            }
            if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
                socklen_t len = sizeof(int);
                int err;
                if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &len) == 0) {
                    // 连接成功
                    if (err == 0) {
                        break;
                    }
                    errno = err;
                }
            }
            if (usec > 0) {
                usec = usec - (timeout.tv_sec*1000000 + timeout.tv_usec);
                if (usec <= 0) {
                    errno = ETIMEDOUT;
                    close(sockfd);
                    error = true;
                    break;
                }
            }
        }
        ptr = ptr->ai_next;
    }
    freeaddrinfo(result);
    if (ptr == NULL || error) {
        return -1;
    }
    set_nonblock(sockfd, false);
    return sockfd;
}

int tcp_listen(const char *hostname, const char *service, int backlog, socklen_t *addrlenp)
{
    struct addrinfo *result;
    result = host_serv(hostname, service, AF_UNSPEC, SOCK_STREAM, AI_PASSIVE);
    if (result == NULL) {
        return -1;
    }
    struct addrinfo *ptr = result;
    int listenfd;
    while (ptr != NULL) {
        listenfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (listenfd >= 0) {
            int on = 1;
            setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
            if (bind(listenfd, ptr->ai_addr, ptr->ai_addrlen) == 0) {
                if (listen(listenfd, backlog) == 0) {
                    break;
                }
            }
            close(listenfd);
        }
        ptr = ptr->ai_next;
    }
    if (ptr == NULL) {
        return -1;
    }
    if (addrlenp != NULL) {
        *addrlenp = ptr->ai_addrlen;
    }
    return listenfd;
}

int readable_time(int fd, int sec) {
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(fd, &rset);
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = 0;
    int ret;
    do {
        ret = select(fd+1, &rset, NULL, NULL, &tv);
    } while (ret == -1 && errno == EINTR);
    return ret;
}

int set_nonblock(int connfd, bool op)
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

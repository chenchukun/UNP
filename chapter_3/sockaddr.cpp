//
// Created by chenchukun on 17/11/26.
//
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <assert.h>
using namespace std;

void ipv4()
{
    cout << "=====ipv4=====" << endl;
    sockaddr_in sockaddrIn;
    bzero(&sockaddrIn, sizeof(sockaddrIn));
    sockaddrIn.sin_family = AF_INET;
    // 设置为统配IP,相当于0.0.0.0
    sockaddrIn.sin_addr.s_addr = INADDR_ANY;
    // 使用IP地址设置sin_addr
    assert(inet_pton(AF_INET, "127.0.0.1", &sockaddrIn.sin_addr)==1);
    // 设置端口
    sockaddrIn.sin_port = htons(6180);
    uint32_t local = 0x7F000001;
    cout << "local = " << local << endl;
    cout << "htonl(local) = " << htonl(local) << endl;
    // sockaddr_in.sin_addr.s_addr保存IP网络字节序的IP地址
    cout << "sockaddrIn.sin_addr.s_addr = " << sockaddrIn.sin_addr.s_addr << endl;
}

void ipv6()
{
    cout << "=====ipv6=====" << endl;
    sockaddr_in6 sockaddrIn6;
    sockaddrIn6.sin6_family = AF_INET6;
    sockaddrIn6.sin6_addr = IN6ADDR_ANY_INIT;
    sockaddrIn6.sin6_port = htons(6180);
}

void sockaddrStorage()
{
    cout << "=====sockaddrStorage=====" << endl;
    // sockaddr_storage可以容纳任意类型的套接字地址
    sockaddr_storage sockaddr;
    sockaddr_in *sockaddrIn = reinterpret_cast<sockaddr_in*>(&sockaddr);
    sockaddrIn->sin_family = AF_INET;
    sockaddrIn->sin_addr.s_addr = INADDR_ANY;
    sockaddrIn->sin_port = htons(6180);
    cout << "sockaddrIn->sin_family = " << sockaddrIn->sin_family << endl;
    cout << "sockaddr.ss_family = " << sockaddr.ss_family << endl;
}

void pton()
{
    cout << "=====pton=====" << endl;
    int ret = 0;
    sockaddr_in sockaddrIn;
    bzero(&sockaddrIn, sizeof(sockaddrIn));
    ret = inet_pton(AF_INET, "127.0.0.1", &sockaddrIn.sin_addr);
//    ret = inet_pton(AF_INET, "127.0.0.1.1", &sockaddrIn.sin_addr);
//    ret = inet_pton(100, "127.0.0.1", &sockaddrIn.sin_addr);
    if (ret == 1) {
        cout << "inet_pton调用成功" << endl;
    }
    else if (ret == 0){
        cerr << "inet_pton调用失败:地址格式有误" << endl;
    }
    else if (ret == -1){
        if (errno == EAFNOSUPPORT) {
            cerr << "inet_pton调用失败:不支持的协议类型" << endl;
        }
        else {
            cerr << "inet_pton调用失败" << endl;
        }
    }
}

void ntop()
{
    cout << "=====ntop=====" << endl;
    sockaddr_in sockaddrIn;
    bzero(&sockaddrIn, sizeof(sockaddrIn));
    assert(inet_pton(AF_INET, "127.0.0.1", &sockaddrIn.sin_addr)==1);

    char ip[16];
    const char *ptr = inet_ntop(AF_INET, &sockaddrIn.sin_addr, ip, sizeof(ip));
//    const char *ptr = inet_ntop(AF_INET, &sockaddrIn.sin_addr, ip, sizeof(ip)-10);
    if (ptr != NULL) {
        cout << "inet_ntop调用成功:" << ptr << endl;
    }
    else if (errno == ENOSPC) {
        cout << "inet_ntop调用失败:" << "空间不够" << endl;
    }
    else  {
        cout << "inet_ntop调用失败" << endl;
    }
}

// 判断主机字节序
void endian()
{
    cout << "=====字节序=====" << endl;
    short x = 0x0102;
    char *p = reinterpret_cast<char*>(&x);
    if (p[0] == 0x02) {
        cout << "little-endian" << endl;
    }
    else if (p[0] == 0x01) {
        cout << "big-endian" << endl;
    }
}

char* sock_ntop(const struct sockaddr *addr, char *host, int len)
{
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

void sockNtopTest()
{
    cout << "=====sockNtopTest=====" << endl;
    sockaddr_in sockaddrIn;
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_addr.s_addr = INADDR_ANY;
    sockaddrIn.sin_port = htons(6180);
    char host[128];
    if (sock_ntop(reinterpret_cast<sockaddr*>(&sockaddrIn), host, sizeof(host)) == NULL) {
        cout << "sock_ntop调用失败" << endl;
        return;
    }
    cout << "sock_ntop调用成功:" << host << endl;
    sockaddr_in6 sockaddrIn6;
    bzero(&sockaddrIn6, sizeof(sockaddrIn6));
    sockaddrIn6.sin6_family = AF_INET6;
    sockaddrIn6.sin6_addr = IN6ADDR_ANY_INIT;
    sockaddrIn6.sin6_port = htons(6180);
    if (sock_ntop(reinterpret_cast<sockaddr*>(&sockaddrIn6), host, sizeof(host)) == NULL) {
        cout << "sock_ntop调用失败" << endl;
        return;
    }
    cout << "sock_ntop调用成功:" << host << endl;
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
    }
    return -1;
}

void testUnp()
{
    cout << "=====testUnp=====" << endl;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int port = sock_bind_wild(sockfd, AF_INET);
    cout << "port = " << port << endl;
}


int main()
{
    endian();
    pton();
    ntop();
    ipv4();
    ipv6();
    sockaddrStorage();
    sockNtopTest();
    testUnp();
    return 0;
}
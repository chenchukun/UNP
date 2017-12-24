//
// Created by chenchukun on 17/12/22.
//

#include <iostream>
#include <netdb.h>
#include "utility.h"
#include "sock.h"
#include <cstring>
#include <arpa/inet.h>
using namespace std;

void printAddrinfo(struct addrinfo *result, int index=0)
{
    string host;
    cout << "addrinfo[0] = " << endl;
    cout << "\tai_flags = " << result->ai_flags << endl;
    cout << "\tai_family = " << family_to_name(result->ai_family) << "(" << result->ai_family << ")" << endl;
    cout << "\tai_socktype = " << socktype_to_name(result->ai_socktype) << "(" << result->ai_socktype << ")" << endl;
    cout << "\tai_protocol = " << result->ai_protocol << endl;
    cout << "\tai_addrlen = " << result->ai_addrlen << endl;
    cout << "\tai_canonname = " << (result->ai_canonname==NULL?"":result->ai_canonname) << endl;
    if (sock_ntop(result->ai_addr, host) == NULL) {
        PrintStdError("sock_ntop:");
        exit(errno);
    }
    cout << "\tai_addr = " << host << endl;
    if (result->ai_next != NULL) {
        printAddrinfo(result->ai_next, index+1);
    }
}

int main(int argc, char **argv)
{
    char *hostname = NULL;
    char *servname = argv[2];
    if (strcmp(argv[1], "NULL") != 0) {
        hostname = argv[1];
    }
    struct addrinfo *result;
    struct addrinfo hints;
    /*
     * getaddrinfo()函数能够处理名字到地址以及服务到端口这两种转换,同时支持IPv4和IPv6
     * hostname传入主机名或IP地址或NULL(统配地址,用于服务端), service传入服务名或十进制端口号串
     * hints可以为NULL,若传入一个addrinfo结构,
     * 通过设置ai_flags、ai_family、ai_socktype、ai_protocol用来指示期待返回的信息类型
     * result 返回一个动态分配的addrinfo链表
     * 成功返回0,失败返回非0错误码
     */
    hints.ai_flags = AI_PASSIVE | AI_CANONNAME; // AI_PASSIVE指示套接字用于被动打开, 指示返回主机的规范名称
    hints.ai_family = AF_INET;             // 指示只返回IPv4的地址
    hints.ai_socktype = SOCK_STREAM;       // 指示只返回TCP的地址
    hints.ai_protocol = 0;
    int ret = getaddrinfo(hostname, servname, &hints, &result);
    if (ret != 0) {
        cout << gai_strerror(ret) << endl;
        exit(ret);
    }
    printAddrinfo(result);
    // 释放getaddrinfo动态分配的addrinfo链表
    freeaddrinfo(result);
    return 0;
}
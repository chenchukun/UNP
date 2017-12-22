//
// Created by chenchukun on 17/12/22.
//

#include <iostream>
#include <netdb.h>
#include "utility.h"
#include "sock.h"
#include <arpa/inet.h>
using namespace std;

void printServent(struct servent *sptr)
{
    cout << "s_name = " << sptr->s_name << endl;
    cout << "s_aliases = " << endl;
    for (int i=0; sptr->s_aliases[i]!=NULL; ++i) {
        cout << "\t" << sptr->s_aliases[i] << endl;
    }
    // 返回的端口为网络字节序
    cout << "s_port = " << ntohs(sptr->s_port) << endl;
    cout << "s_proto = " << sptr->s_proto << endl;
}

int main(int argc, char **argv)
{
    char *proto = NULL;
    if (argc > 2) {
        proto = argv[2];
    }
    // 通过服务名获取服务端口,/etc/services保存着服务名和端口的映射
    // proto指定服务协议类型,可以说udp或tcp等
    struct servent *sptr = getservbyname(argv[1], proto);
    if (sptr == NULL) {
        cout << "getservbyname: " << hstrerror(h_errno) << endl;
        exit(h_errno);
    }
    printServent(sptr);
    cout << endl;
    // 根据端口号获取服务名,传入的是网络字节序的端口号
    sptr = getservbyport(sptr->s_port, proto);
    if (sptr == NULL) {
        cout << "getservbyport: " << hstrerror(h_errno) << endl;
        exit(h_errno);
    }
    printServent(sptr);
    return 0;
}
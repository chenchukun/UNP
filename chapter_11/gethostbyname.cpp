//
// Created by chenchukun on 17/12/21.
//

#include <iostream>
#include <netdb.h>
#include "utility.h"
#include "sock.h"
#include <arpa/inet.h>
using namespace std;

int main(int argc, char **argv)
{
    // 通过主机名获取IP地址,成功返回hostent指针,失败返回NULL,并设置h_errno
    struct hostent *hptr = gethostbyname(argv[1]);
    if (hptr == NULL) {
        cout << "gethostbyname: " << hstrerror(h_errno) << endl;
        exit(h_errno);
    }
    cout << "h_name = " << hptr->h_name << endl;    // 主机规范名字
    cout << "h_aliases = " << endl;                 // 别名列表
    for (int i=0; hptr->h_aliases[i] != NULL; ++i) {
        cout << "\t" << hptr->h_aliases[i] << endl;
    }
    // 地址类型
    cout << "h_addrtype = " << family_to_name(hptr->h_addrtype) << "(" << hptr->h_addrtype << ")"<< endl;
    // 地址长度
    cout << "h_length = " << hptr->h_length << endl;
    // 地址列表, in_addr 类型, sockaddr_in.sin_addr
    cout << "h_addr_list = " << endl;
    char host[128];
    for (int i=0; hptr->h_addr_list[i] != NULL; ++i) {
        cout << "\t" << inet_ntop(hptr->h_addrtype, hptr->h_addr_list[i], host, sizeof(host)) << endl;
    }
    return 0;
}
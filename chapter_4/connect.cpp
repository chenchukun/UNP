//
// Created by chenchukun on 17/11/28.
//

#include <sys/socket.h>
#include <iostream>
#include <memory>
#include <assert.h>
#include "unp.h"
using namespace std;

int main(int argc, char **argv)
{
    const char *ip;
    int port;
    if (argc < 2) {
        cerr << "connect ip port" << endl;
        exit(1);
    }
    port = stoi(argv[2]);
    ip = argv[1];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    shared_ptr<int> raii(&sockfd, [](int *p){close(*p);});
    assert(sockfd >= 0);
    sockaddr_in addr;
    assert(sockaddr_set(&addr, AF_INET, port, ip)==0);
    char buff[128];
    const char *host = sock_ntop(&addr, buff, sizeof(buff));
    if (host == NULL) {
        cerr << "转换失败" << endl;
        exit(1);
    }
    // 连接TCP服务器,将发起三次握手,成功返回0,失败返回-1
    // 在尝试建立连接失败后,套接字描述符不可再使用,必须关闭,若需要重试,必须新建套接字描述符
    int ret = connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (ret == 0) {
        cout << "连接TCP服务器" << buff << "成功" << endl;
    }
    else {
        cerr << "连接TCP服务器" << buff << "失败" << endl;
        if (errno == ETIMEDOUT) {
            // 发送SYN后得不到对方的响应,且重试多次都得不到响应
            cerr << "连接超时:" << strerror(errno) << endl;
        }
        else if (errno == ECONNREFUSED) {
            // 对端收到SYN后响应RST,表示对端主机没有指定监听指定端口
            cerr << "目的端口未开放:" << strerror(errno) << endl;
        }
        else if (errno==EHOSTUNREACH || errno==ENETUNREACH) {
            // ICMP错误,由于某个路由器发生错误,会发送ICMP错误,再多次重试失败会会返回该错误
            // 按照本地路由表转发找不到主机或connect调用不等待直接返回也会产生该错误
            cerr << "网络不可达:" << strerror(errno) << endl;
        }
    }
    return 0;
}
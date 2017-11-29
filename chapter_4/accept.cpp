//
// Created by chenchukun on 17/11/29.
//

#include <sys/socket.h>
#include <iostream>
#include <memory>
#include <assert.h>
#include "unp.h"
using namespace std;

int main(int argc, char **argv)
{
    int backlog = 10;
    int lisfd = sock_listen(AF_INET, backlog, 6180);
    assert(lisfd >= 0);
    while (true) {
        sockaddr_storage cliaddr;
        socklen_t addrlen;
        // 从已完成连接队列队头返回一个已完成连接套接字,若队列为空,则阻塞(或返回)
        // cliaddr和addrlen返回客户端的协议地址和长度,若不获取该信息,可以设置为NULL
        // 成功返回新的套接字描述符,失败返回-1
        int sockfd = accept(lisfd, reinterpret_cast<sockaddr*>(&cliaddr), &addrlen);
        if (sockfd >= 0) {
            char buff[128];
            cout << "接收到连接, " << sock_ntop(&cliaddr, buff, sizeof(buff)) << " sockfd = " << sockfd << endl;
            close(sockfd);
        }
        else {
            cout << "接收连接失败: " << strerror(errno) << endl;
        }
    }
    return 0;
}
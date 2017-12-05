//
// Created by chenchukun on 17/11/29.
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

int main(int argc, char **argv)
{
    int backlog = 10;
    int lisfd = sock_listen(AF_INET, backlog, 6180);
    if (lisfd < 0) {
        cerr << "创建监听套接字失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    while (true) {
        sockaddr_storage cliaddr;
        socklen_t addrlen;
        // 从已完成连接队列队头返回一个已完成连接套接字,若队列为空,则阻塞(或返回)
        // cliaddr和addrlen返回客户端的协议地址和长度,若不获取该信息,可以设置为NULL
        // 成功返回新的套接字描述符,失败返回-1
        // 在三次握手完成后,在服务器未调用accept之前从该连接到达的数据将在TCP接收缓冲区中排队,
        // 排队的最大数据量为缓冲区大小(疑问:若缓冲区慢后,对端接收到窗口值为0,将无法发送数据,那什么时候可以恢复发送?)
        int sockfd = accept(lisfd, reinterpret_cast<sockaddr*>(&cliaddr), &addrlen);
        if (sockfd >= 0) {
            char buff[128];
            cout << "接收到连接, " << sock_ntop(&cliaddr, buff, sizeof(buff)) << " sockfd = " << sockfd << endl;
            close(sockfd);
        }
        else if (errno == EINVAL) {
            cerr << "接收连接失败,不是监听套接字:" << strerror(errno) << endl;
            break;
        }
        // 被信号中断时errno为EINTR,三次握手完成后,套接字进入已连接队列排队,此时客户端发送RST会产生ECONNABORTED错误
        else if (errno == EINTR || errno == ECONNABORTED) {
            continue;
        }
        else {  // 其他致命错误
            cerr << "接收连接失败: " << strerror(errno) << endl;
            break;
        }
    }
    return 0;
}
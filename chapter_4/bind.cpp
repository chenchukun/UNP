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
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    shared_ptr<int> raii(&sockfd, [](int *p){close(*p);});
    assert(sockfd >= 0);
    sockaddr_in addr;
    assert(sockaddr_set(&addr, AF_INET, 0) == 0);
    // 将一个协议地址绑定到套接字描述符上,成功返回0,失败返回-1
    // 若指定端口号为0,内核将选择一个临时端口
    // 若IP地址为统配地址,内核将在套接字已连接(TCP)或在套接字上发送数据报(UDP)时选择一个本地IP地址
    // 若没有调用bind,在调用connect或listen后,内核将为该套接字选择一个临时端口
    // 客户端一般不调用bind,它由内核分配一个临时端口,并由内核根据外出网络接口选择源IP地址
    int ret = ::bind(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (ret == 0) {
        cout << "绑定地址成功" << endl;
    }
    else {
        cerr << "绑定失败" << endl;
        if (errno == EADDRINUSE) {
            cerr << "地址已使用:" << strerror(errno) << endl;
        }
        exit(1);
    }
    socklen_t len = sizeof(addr);
    // 通过getsockname来获得绑定的协议地址
    // 若要内核选择临时端口号,临时端口号是在bind调用时确定,而IP不在bind时确定
    assert(getsockname(sockfd, reinterpret_cast<sockaddr*>(&addr), &len) == 0);
    char buff[128];
    assert(sock_ntop(&addr, buff, sizeof(buff)) != NULL);
    cout << buff << endl;
    return 0;
}
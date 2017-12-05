//
// Created by chenchukun on 17/11/30.
//

#include <sys/socket.h>
#include <iostream>
#include <memory>
#include <assert.h>
#include <ctime>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include<sys/wait.h>
#include "sock.h"
#include "utility.h"
using namespace std;
// 使用最简单的fork来实现并发时间服务器


void timeService(int sockfd)
{
    time_t t = time(NULL);
    const char *data = ctime(&t);
    size_t len = strlen(data);
    if (write(sockfd, data, len) != len) {
        cerr << "发送数据失败:" << "(" << errno << ")" << endl;
    }
}

int main()
{
    int lisfd = sock_listen(AF_INET, 10, 6180);
    if (lisfd < 0) {
        cerr << "创建监听套接字失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    while (true) {
        int sockfd = accept(lisfd, NULL, NULL);
        if (sockfd < 0) {
            cerr << "accept调用失败:" << "(" << errno << ")" << endl;
            continue;
        }
        pid_t pid = fork();
        if (pid == -1) {
            cerr << "创建子进程失败:" << strerror(errno) << "(" << errno << ")" << endl;
        }
        else if(pid == 0) {
            // 子进程关闭监听套接字
            // 文件描述符具有引用计数,调用fork()会增加父进程打开的文件描述符的引用计数
            // 调用close()时,会将引用计数减一,若引用计数减到零,则发送FIN,关闭连接。
            // 调用close()后不能在对该描述符进行操作,TCP会尝试发送已在发送缓冲区中的数据(不能接收),
            // 发送完毕后发送FIN, 调用shutdown可以立即发送FIN,而不理会引用计数
            close(lisfd);
            timeService(sockfd);
            close(sockfd);
            exit(0);
        }
        close(sockfd);
    }
    return 0;
}

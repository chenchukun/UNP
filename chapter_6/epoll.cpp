//
// Created by chenchukun on 17/12/8.
//

#include <iostream>
#include <string>
#include <getopt.h>
#include <sys/socket.h>
#include <memory>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <thread>
#include "sock.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <set>
#include "utility.h"
using namespace std;

int main()
{
    int listenfd = sock_listen(AF_INET, 10, 6180);
    if (listenfd < 0) {
        PrintStdError("创建监听套接字失败");
        exit(1);
    }
    char buff[4096];
    int epfd = epoll_create(1024);
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    if (epfd < 0) {
        PrintStdError("创建epoll实例失败");
        close(listenfd);
        exit(1);
    }
    int fdCount = 1;
    vector<epoll_event> evlist;
    evlist.resize(10);
    while (true) {
        int nready = epoll_wait(epfd, &evlist[0], evlist.size(), -1);
        if (nready == -1) {
            if (errno == EINTR) {
                continue;
            }
            PrintStdError("epoll错误");
            break;
        }
        for (int i=0; i<nready; ++i) {
            int fd = evlist[i].data.fd;
            int event = evlist[i].events;
            if (fd == listenfd) {
                int connfd = accept(fd, NULL, NULL);
                // 添加新感兴趣套接字描述符
                if (connfd >= 0) {
                    ev.events = EPOLLIN;
                    ev.data.fd = connfd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                    cout << "客户端上线" << endl;
                    if (++fdCount > evlist.size()) {
                        evlist.resize(evlist.size()*2);
                    }
                }
                else {
                    PrintStdError("获取连接失败");
                }
            }
            else {
                int n = read(fd, buff, sizeof(buff));
                if (n == 0) {
                    cout << "客户端下线" << endl;
                }
                else if (n < 0) {
                    PrintStdError("读取数据失败");
                }
                else {
                    writen(fd, buff, n);
                    continue;
                }
                // 关闭套接字会自动从epoll实例的感兴趣列表中移除
                close(fd);
                --fdCount;
            }
        }
    }
    close(listenfd);
    close(epfd);
    return 0;
}
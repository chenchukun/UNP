//
// Created by chenchukun on 17/12/6.
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
#include <poll.h>
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
    vector<pollfd> fds;
    vector<int> idle;
    char buff[4096];
    fds.push_back(pollfd());
    pollfd &fd = fds.back();
    fd.fd = listenfd;
    fd.events = POLL_IN;
    while (true) {
        // 错误返回-1,定时器到期返回0,否则返回就绪描述符个数。
        // timeout单位为毫秒,负数表示永远阻塞,0表示不阻塞立即返回
        int nready = poll(&fds[0], fds.size(), -1);
        if (nready == -1) {
            if (errno == EINTR) {
                continue;
            }
            PrintStdError("poll错误");
            break;
        }
        if (fds[0].revents & POLL_IN) {
            int connfd = accept(fds[0].fd, NULL, NULL);
            if (connfd >= 0) {
                if (!idle.empty()) {
                    pollfd &conn = fds[idle.back()];
                    idle.pop_back();
                    conn.fd = connfd;
                    conn.events = POLL_IN;
                }
                else {
                    fds.push_back(pollfd());
                    pollfd &fd = fds.back();
                    fd.fd = connfd;
                    fd.events = POLL_IN;
                }
                cout << "客户端上线" << endl;
            }
            else {
                PrintStdError("获取连接失败");
            }
            --nready;
        }
        for (int i=1; i<fds.size() && nready>0; ++i) {
            if (fds[i].fd == -1) {
                continue;
            }
            pollfd &fd = fds[i];
            if (fd.revents & POLL_IN) {
                --nready;
                int n = read(fd.fd, buff, sizeof(buff));
                if (n == 0) {
                    cout << "客户端下线" << endl;
                }
                else if (n < 0) {
                    PrintStdError("读取数据失败");
                }
                else {
                    writen(fd.fd, buff, n);
                    continue;
                }
                close(fd.fd);
                fd.fd = -1;
                idle.push_back(i);
            }
        }
    }
    close(listenfd);
    return 0;
}
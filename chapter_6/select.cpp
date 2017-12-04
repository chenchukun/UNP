//
// Created by chenchukun on 17/12/3.
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
#include "unp.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <set>
#include "utility.h"
using namespace std;

/*
 * 描述符读就绪条件
 */
void readReady()
{
    set<int> fdset;
    char buff[4096];
    fd_set rset, wset, eset;
    struct timeval tv;
    int maxfd = 0;
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_ZERO(&eset);
    int listenfd = sock_listen(AF_INET, 10, 6180);
    if (listenfd < 0) {
        PrintStdError("创建监听套接字失败");
        exit(1);
    }
    fdset.insert(listenfd);
    while (true) {
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_ZERO(&eset);
        auto it = fdset.begin();
        while (it != fdset.end()) {
            FD_SET(*it, &rset);
            ++it;
        }
        maxfd = *(--fdset.end());
        int ret = select(maxfd + 1, &rset, &wset, &eset, &tv);
        if (ret == 0) {
            cout << "当前在线用户数:" << fdset.size() - 1 << endl;
        }
        if (ret == -1 && errno == EINTR) {
            continue;
        }
        // 套接字可读的4个条件
        for (int fd=0; fd<=maxfd && ret>0; ++fd) {
            if (FD_ISSET(fd, &rset)) {
                // 1、当监听套接字有已完成的连接时,套接字可读
                if (listenfd == fd) {
                    // 阻塞IO,可能会导致阻塞
                    int sockfd = accept(listenfd, NULL, NULL);
                    if (sockfd < 0) {
                        PrintStdError("接收连接失败");
                        continue;
                    }
                    fdset.insert(sockfd);
                    cout << "客户端 " << sockfd << " 上线" << endl;
                }
                else {
                    int n = read(fd, buff, sizeof(buff));
                    if (n == 0) {
                        // 2、连接读半部分关闭,即接收到FIN,套接字可读,返回0
                        cout << "客户端 " << fd << " 下线" << endl;
                        fdset.erase(fd);
                        close(fd);
                    }
                    else if (n == -1) {
                        // 3、套接字上有错误待处理,此时套接字可读,返回-1,并设置errno为指定错误。
                        // 也可以通过指定SO_ERROR套接字选项调用getsockopt获取并清除。
                        PrintStdError("从读取数据失败");
                        fdset.erase(fd);
                        close(fd);
                    }
                    else {
                        // 4、套接字接收缓冲区中数据字节数大于登录套接字接收缓冲区低水位标记的当前大小
                        // 套接字可读,低水位标志默认大小为1,可以通过套接字选项SO_RCVLOWAT设置
                        buff[n] = 0;
                        cout << "从客户端读取 " << n << " 字节数据:" << buff << endl;
                    }
                }
                --ret;
            }
        }
    }
}

/*
 * 描述符写就绪条件
 */
void writeReady()
{
    set<int> fdset;
    const char *msg = "hello world\r\n";
    fd_set rset, wset, eset;
    int maxfd = 0;
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_ZERO(&eset);
    int listenfd = sock_listen(AF_INET, 10, 16180);
    if (listenfd < 0) {
        PrintStdError("创建监听套接字失败");
        exit(1);
    }
    fdset.insert(listenfd);
    while (true) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_ZERO(&eset);
        auto it = fdset.begin();
        while (it != fdset.end()) {
            if (*it == listenfd) {
                FD_SET(*it, &rset);
            }
            else {
                FD_SET(*it, &wset);
            }
            ++it;
        }
        maxfd = *(--fdset.end());
        // 模拟连接写半部分关闭
        if (fdset.find(5) != fdset.end()) {
            shutdown(5, SHUT_WR);
        }
        int ret = select(maxfd + 1, &rset, &wset, &eset, NULL);
        if (ret == -1 && errno == EINTR) {
            continue;
        }
        for (int fd=0; fd<=maxfd && ret>0; ++fd) {
            if (FD_ISSET(fd, &rset)) {
                int sockfd = accept(listenfd, NULL, NULL);
                if (sockfd < 0) {
                    PrintStdError("接收连接失败");
                    continue;
                }
                fdset.insert(sockfd);
                cout << "客户端 " << sockfd << " 上线" << endl;
                --ret;
            }
            // 套接字可写的4个条件
            if (FD_ISSET(fd, &wset)) {
                int n = writen(fd, msg, strlen(msg));
                if (n == strlen(msg)) {
                    // 1、套接字发送缓冲区可用空间大于等于套接字发送缓冲区低水位标准的当前大小,
                    // 并且该套接字已连接,或不需要连接(UDP),此时写入将返回真值,表示写入的字节数。
                    // 可用使用SO_SNDLOWAT套接字选项来设置套接字的低水位标准,默认为2048
                    cout << "发送 " << n << " 字节数据到给客户端 " << fd << " 成功:" << msg << endl;
                }
                else if (n == -1) {
                    // 2、连接写半部分关闭,次数写入将产生SIGPIPE信号
                    // 3、套接字上有错误待处理,此时套接字可写,返回-1,并设置errno为指定错误。
                    PrintStdError("写套接字发送错误");
                    cout << "客户端 " << fd << " 下线" << endl;
                    close(fd);
                    fdset.erase(fd);
                }
                // 4、使用非阻塞式connect的套接字已建立连接,或者connect已经以失败告终,套接字可写。
                --ret;
            }
        }
    }
}

/*
 * 异常就绪条件: 如果一个套接字存在带外数据或者仍处于带外标记,那么它有异常条件待处理
 */

void sigPipe(int signo)
{
    cout << "PIPE" << endl;
}

int main()
{
    // 常量FD_SETSIZE定义了fd_set中的描述符总数,通常为1024
    cout << "FD_SETSIZE = " << FD_SETSIZE << endl;
    signal_action(SIGPIPE, sigPipe);
    thread t1(readReady);
    thread t2(writeReady);
    t1.join();
    t2.join();
    return 0;
}
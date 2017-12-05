//
// Created by chenchukun on 17/12/1.
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
#include "sock.h"
#include <signal.h>
#include <sys/wait.h>
#include "utility.h"
using namespace std;

/*
 * 主动关闭状态转换:FIN_WAIT_1  FIN_WAIT_2 TIME_WAIT
 * 被动关闭状态转换:CLOSE_WAIT  LAST_ACK   CLOSED
 */

enum Type{ECHO_CLIENT, ECHO_SERVICE};

struct Option
{
    string ip;
    uint16_t port;
    Type type;
};

int parseCommandLine(int argc, char **argv, Option &option)
{
    int opt;
    while ((opt = getopt(argc, argv, "b:cs")) != -1) {
        switch(opt) {
            case 'b': {
                vector <string> host = StrSplit(optarg, ":");
                option.ip = host[0];
                option.port = host.size()>1? stoi(host[1]): 0;
                break;
            }
            case 'c':
                option.type = ECHO_CLIENT;
                break;
            case 's':
                option.type = ECHO_SERVICE;
                break;
            default:
                return -1;
        }
    }
    return 0;
}

void help() {
    cout << "echo -b ip:port [-s] [-c]" << endl;
    exit(1);
}

void echo(int connfd, const string &host)
{
    ssize_t n;
    char buff[1024];
    while (true) {
        // 客户端正常关闭连接时返回0
        n = read(connfd, buff, sizeof(buff));
        if (n == 0) {
            cout << host << "客户端关闭连接" << endl;
            break;
        }
        if (n > 0) {
            if (writen(connfd, buff, n) != n) {
                cerr << "发送数据错误:" << strerror(errno) << "(" << errno << ")" << endl;
                break;
            }
            continue;
        }
        if (errno == EINTR) {
            continue;
        }
        cerr << "echo: 读取数据失败:" << strerror(errno) << "(" << errno << ")" << endl;
        break;
    }
    close(connfd);
}

void sigChld(int signo)
{
    pid_t pid;
    int stat;
    // -1表示等待第一个终止的子进程
    // WNOHANG表示在没有已终止进程是不阻塞
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0);
}

void echoSrv(const char *ip, uint16_t port)
{
    int lisfd = sock_listen(AF_INET, 10, port, ip);
    shared_ptr<int> raii(&lisfd, [](int *p){close(*p);});
    if (lisfd < 0) {
        cerr << "创建监听套接字失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    // 处理子进程退出参数的SIGCHLD信号,回收僵尸进程
    if (signal_action(SIGCHLD, sigChld) == SIG_ERR) {
        cerr << "设置SIGCHLD信号处理函数失败:" << strerror(errno) << "(" << errno << ")" << endl;
    }
    cout << "echo服务启动:" << ip << ":" << port << endl;
    sockaddr_in cliaddr;
    socklen_t clilen;
    string host;
    while (true) {
        clilen = sizeof(cliaddr);
        int connfd = accept(lisfd, reinterpret_cast<sockaddr*>(&cliaddr), &clilen);
        if (connfd < 0) {
            // 非致命错误:处理信号中断 和 ECONNABORTED
            if (errno == EINTR || errno == ECONNABORTED) {
                continue;
            }
            // 致命错误
            cerr << "接收连接失败:" << strerror(errno) << "(" << errno << ")" << endl;
            break;
        }
        if (sock_ntop(&cliaddr, host) == NULL) {
            cerr << "获取客户端地址失败:" << strerror(errno) << "(" << errno << ")" << endl;
            close(connfd);
            continue;
        }
        cout << host << "上线" << endl;
        pid_t pid = fork();
        if (pid == -1) {
            cerr << "创建子进程失败:""接收连接失败:" << strerror(errno) << "(" << errno << ")" << endl;
            close(connfd);
            continue;
        }
        if (pid == 0) {
            close(lisfd);
            echo(connfd, host);
            exit(0);
        }
        close(connfd);
    }
    close(lisfd);
}

void echoCli(const char *ip, uint16_t port)
{
    int connfd = sock_connect(AF_INET, port, ip);
    if (connfd == -1) {
        cerr << "连接服务器" << ip << ":" << port << "失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    char buff[4096];
    ssize_t n;
    fd_set rset;
    int maxfd = connfd + 1;
    bool stdineof = false;
    while (true) {
        FD_ZERO(&rset);
        // 标准输入为输入完成
        if (!stdineof) {
            FD_SET(STDIN_FILENO, &rset);
        }
        FD_SET(connfd, &rset);
        int ret = select(maxfd, &rset, NULL, NULL, NULL);
        if (ret == -1 && errno == EINTR) {
            continue;
        }
        // 读取标准IO
        if (FD_ISSET(STDIN_FILENO, &rset)) {
            int n = read(STDIN_FILENO, buff, sizeof(buff));
            // 标准输入完成,不能立即退出,应该关闭写端,等待服务器发送数据完成后关闭连接
            if (n == 0) {
                stdineof = true;
                // 关闭写,表示不再往套接字中写数据
                shutdown(connfd, SHUT_WR);
            }
            else if (n > 0) {
                if (writen(connfd, buff, n) < 0) {
                    PrintStdError("发送数据错误");
                }
            }
            else {
                PrintStdError("读取标准输入发生错误");
            }
        }
        // 读取套接字
        if (FD_ISSET(connfd, &rset)) {
            int n = read(connfd, buff, sizeof(buff));
            if (n == 0) {
                if (stdineof == true) {
                    cout << "服务器关闭连接" << endl;
                }
                else {
                    cerr << "服务器提前下线" << endl;
                }
                break;
            }
            else if (n > 0) {
                write(STDOUT_FILENO, buff, n);
            }
            else {
                PrintStdError("接收数据错误");
                break;
            }
        }
    }
}

int main(int argc, char **argv)
{
    Option option;
    if (parseCommandLine(argc, argv, option) != 0) {
        help();
    }
    // 忽略SIGPIPE信号,在写一个已接收到RST的套接字时会产生该信号,默认为终止进程
    signal_action(SIGPIPE, SIG_IGN);
    if (option.type == ECHO_SERVICE) {
        echoSrv(option.ip.c_str(), option.port);
    }
    else {
        echoCli(option.ip.c_str(), option.port);
    }
    return 0;
}


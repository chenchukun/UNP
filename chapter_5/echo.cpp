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
    // WNOHANG表示在没有已终止进程时不阻塞
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

/*
 * 存在的问题:
 * 若服务端进程终止时,客户端因阻塞在getlinet()上导致无法及时处理
 * 此时服务端写端关闭,处于FIN_WAIT2状态,等待客户端发送FIN来彻底断开连接
 * 客户端读端关闭,接收到服务器对FIN的ACK,处于CLOSE_WAIT状态
 * 客户端getline()返回后,由于服务端发送来的FIN只能说明服务端不会再往连接中写数据,并不能告知客户端进程已终止
 * 客户端仍然可以调用write()写数据,此时由于服务端已终止,响应RST。
 * 客户端在调用完write()后立即调用read(),
 * 若此时RST未到达,由于服务端已发送了FIN,因此可能立即立即返回0,RST将不会被接收,若RST已到达,则返回ECONNRESET错误
 *
 * 若服务端主机崩溃,客户端不能及时得到反馈
 * 客户端调用write()写入内核缓冲区,TCP次序重传数据,试图从服务器上接收一个ACK。
 * 而客户端将一直阻塞在read()上,直到TCP返回超时ETIMEOUT或目标不可达EHOSTUNREACH、ENETUNREACH
 */
void echoCli(const char *ip, uint16_t port)
{
    int connfd = sock_connect(AF_INET, port, ip);
    if (connfd == -1) {
        cerr << "连接服务器" << ip << ":" << port << "失败:" << strerror(errno) << "(" << errno << ")" << endl;
        exit(1);
    }
    string msg;
    string repert;
    ssize_t n;
    while (getline(cin, msg)) {
        msg += "\n";
        int32_t len = htonl(msg.size());
        writen(connfd, &len, sizeof(len));
        n = writen(connfd, msg.c_str(), msg.size());
        if (n != msg.size()) {
            cerr << "发送数据错误:" << strerror(errno) << "(" << errno << ")" << endl;
            exit(1);
        }
        if (msg.size() > repert.size()) {
            repert.resize(msg.size());
        }
        n = readn(connfd, &repert[0], msg.size());
        if (n == 0) {
            cerr << "服务端过早终止" << endl;
            exit(1);
        }
        else if (n != msg.size()) {
            cerr << "接收数据错误:" << strerror(errno) << "(" << errno << ")" << endl;
        }
        write(STDOUT_FILENO, repert.c_str(), n);
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


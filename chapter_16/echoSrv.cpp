//
// Created by chenchukun on 18/1/7.
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
#include <map>
#include <set>
#include "utility.h"
#include "SimpleBuffer.h"

using namespace std;


struct Context
{
    Context(int fd_, int status_)
            : fd(fd_), status(status_)
    {
    }

    int fd;
    SimpleBuffer readBuff;
    SimpleBuffer writeBuff;
    int status; // 0正常, 1 读关闭,
};

class Epoll
{
public:
    int init(int size=1024) {
        epfd = epoll_create(size);
        if (epfd < 0) {
            return -1;
        }
        evlist.resize(size);
        return 0;
    }

    int addEvent(int fd, uint32_t event) {
        epoll_event ev;
        ev.events = event;
        Context *context = new Context(fd, 0);
        ev.data.ptr = reinterpret_cast<void*>(context);
        return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    }

    int modifyEvent(epoll_event &event) {
        Context *context = reinterpret_cast<Context*>(event.data.ptr);
        return epoll_ctl(epfd, EPOLL_CTL_MOD, context->fd, &event);
    }

    int wait(int timeout=-1) {
        int nready = 0;
        do {
            nready = epoll_wait(epfd, &evlist[0], evlist.size(), timeout);
        }
        while (nready == -1 && errno == EINTR);
        return nready;
    }

    epoll_event& get(size_t index) {
        return evlist[index];
    }

private:
    int epfd;
    vector<epoll_event> evlist;
};


void start(const char *hostname, const char *service)
{
    int listenFd = tcp_listen(hostname, service);
    if (listenFd < 0) {
        PrintStdError("tcp_listen error:");
        exit(1);
    }
    set_nonblock(listenFd, true);
    Epoll epoll;
    if (epoll.init() < 0) {
        PrintStdError("epoll_create:");
        close(listenFd);
        exit(1);
    }
    if (epoll.addEvent(listenFd, EPOLLIN) < 0) {
        PrintStdError("epoll_ctl:");
        close(listenFd);
        exit(1);
    }
    cout << "服务启动成功" << endl;
    while (true) {
        int nready = epoll.wait(-1);
        if (nready < 0) {
            PrintStdError("epoll_wait:");
            continue;
        }
        for (int i=0; i<nready; ++i) {
            epoll_event &ev = epoll.get(i);
            Context *context = reinterpret_cast<Context*>(ev.data.ptr);
            int fd = context->fd;
            if (fd == listenFd) {
                // 接收新连接,并添加到epoll中
                for (int k=0; k<10; ++k) {
                    int sockfd = accept(fd, NULL, NULL);
                    if (sockfd < 0) {
                        PrintStdError("accept:");
                        break;
                    }
                    if (epoll.addEvent(sockfd, EPOLLIN) < 0) {
                        PrintStdError("addEvent:");
                        close(sockfd);
                        continue;
                    }
                    cout << "接收新连接成功, fd = " << sockfd << endl;
                }
            }
            else {
                SimpleBuffer &readBuff = context->readBuff;
                SimpleBuffer &writeBuff = context->writeBuff;
                uint32_t event = ev.events;
                int ret = 0;
                // 可读
                if (event | EPOLLIN) {
                    if (readBuff.writeSize() <= 0) {
                        cout << "接收缓冲区空间不足" << endl;
                    }
                    else {
                        ret = read(fd, readBuff.writeHead(), readBuff.writeSize());
                        // 读取失败,关闭连接
                        if (ret == -1) {
                            if (errno != EINTR && errno != EAGAIN) {
                                PrintStdError("读取数据失败:");
                                close(fd);
                                delete context;
                                continue;
                            }
                        }
                            // 对方关闭连接
                        else if (ret == 0) {
                            // 发送缓冲区无数据,则关闭连接
                            if (writeBuff.readSize() <= 0) {
                                close(fd);
                                cout << "对端关闭连接,服务结束" << endl;
                                delete context;
                                continue;
                            }
                            else {
                                // 发送缓冲区还有数据,则继续发送缓冲区中数据
                                context->status = 1;
                                ev.events = EPOLLOUT;
                                if (epoll.modifyEvent(ev) < 0) {
                                    PrintStdError("modifyEvent:");
                                    close(fd);
                                    delete context;
                                    continue;
                                }
                            }
                        }
                        else if (ret > 0) {
                            cout << "读取数据" << ret << " byte 数据" << endl;
                            if (readBuff.readSize() <= 0) {
                                ev.events = EPOLLIN | EPOLLOUT;
                                if (epoll.modifyEvent(ev) < 0) {
                                    PrintStdError("modifyEvent:");
                                    close(fd);
                                    delete context;
                                    continue;
                                }
                            }
                            strncpy(writeBuff.writeHead(), readBuff.writeHead(), ret);
                            writeBuff.setWritePosition(readBuff.writePosition() + ret);
                        }
                    }
                }
                // 可写
                if (event | EPOLLOUT) {
                    cout << "writeBuff.readSize() = " << writeBuff.readSize() << endl;
                    if (writeBuff.readSize() > 0) {
                        ret = write(fd, writeBuff.readHead(), writeBuff.readSize());
                        if (ret < 0) {
                            if (errno != EAGAIN && errno != EINTR) {
                                close(fd);
                                delete context;
                                PrintStdError("write:");
                                continue;
                            }
                        }
                        else if (ret >= 0) {
                            cout << "写" << ret << " byte 数据" << endl;
                            writeBuff.setReadPosition(writeBuff.readSize() + ret);
                        }
                    }
                    if (writeBuff.readSize() <= 0) {
                        if (context->status == 0) {
                            ev.events = EPOLLIN;
                            if (epoll.modifyEvent(ev) < 0) {
                                PrintStdError("modifyEvent:");
                                close(fd);
                                delete context;
                                continue;
                            }
                            cout << "数据发送完毕,取消可写事件" << endl;
                        }
                        else {
                            close(fd);
                            delete context;
                            cout << "数据发送完毕,关闭连接" << endl;
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        cerr << "parameter error: echoSrv hostname service" << endl;
        exit(1);
    }
    start(argv[1], argv[2]);
    return 0;
}
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
#include <set>
#include "utility.h"

using namespace std;

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
        ev.data.fd = fd;
        return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    }

    int wait(int timeout=-1) {
        int nready = epoll_wait(epfd, &evlist[0], evlist.size(), timeout);

    }

private:
    int epfd;
    vector<epoll_event> evlist;
};

int main(int argc, char **argv)
{
    if (argc < 3) {
        cerr << "parameter error: echoSrv hostname service" << endl;
        exit(1);
    }
    int listenFd = tcp_listen(argv[1], argv[2]);
    if (listenFd < 0) {
        PrintStdError("tcp_listen error:");
        exit(1);
    }
    set_nonblock(listenFd, true);

}
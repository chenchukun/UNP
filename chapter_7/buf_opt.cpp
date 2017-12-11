//
// Created by chenchukun on 17/12/11.
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
#include <sys/time.h>
#include <assert.h>
#include <set>
#include "utility.h"
using namespace std;

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_listen(listenfd, AF_INET, 10, 6180) == -1) {
        PrintStdError("创建监听套接字失败");
        exit(1);
    }
    int optval;
    socklen_t len = sizeof(optval);
    int ret = getsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, &optval, &len);
    assert(ret == 0);
    cout << "SO_RCVBUF = " << optval << endl;
    return 0;
}
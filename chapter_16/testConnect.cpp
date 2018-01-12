//
// Created by chenchukun on 18/1/12.
//

#include "sock.h"
#include <iostream>
#include <string>
#include "utility.h"
using namespace std;

int main(int argc, char **argv)
{
    int sockfd = tcp_connect_nonblock(argv[1], argv[2], stoi(argv[3]));
    if (sockfd >= 0) {
        cout << "连接成功" << endl;
        close(sockfd);
    }
    else {
        PrintStdError("连接失败");
    }
    return 0;
}
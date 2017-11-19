//
// Created by chenchukun on 17/11/1.
//
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include "../com/libary.h"
using namespace std;

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6180);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        cerr << "connect error " << endl;
        exit(-1);
    }
    struct sockaddr_storage saddr;
    bzero(&saddr, sizeof(saddr));
    socklen_t socklen;
    CHECK_RETURN_EXIT(getsockname(fd, reinterpret_cast<sockaddr*>(&saddr), &socklen));
    char ip[60];
    CHECK_RETURN_EXIT(inet_ntop(AF_INET, reinterpret_cast<sockaddr*>(&saddr), ip, sizeof(ip)));
    cout << ip << endl;
    bzero(&saddr, sizeof(saddr));
    CHECK_RETURN_EXIT(getpeername(fd, reinterpret_cast<sockaddr*>(&saddr), &socklen));
    CHECK_RETURN_EXIT(inet_ntop(AF_INET, reinterpret_cast<sockaddr*>(&saddr), ip, sizeof(ip)));
    cout << ip << endl;
    char buff[64];
    int offset = 0;
    int len;
    while ((len = read(fd, buff + offset, sizeof(buff)-offset)) >= 0) {
        if (len == 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        offset += len;
        buff[offset] = 0;
    }
    if (len < 0) {
        cerr << "read error" << endl;
        exit(-1);
    }
    cout << buff << endl;
    return 0;
}
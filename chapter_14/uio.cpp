//
// Created by chenchukun on 17/12/25.
//

#include "sock.h"
#include "utility.h"
#include <getopt.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/uio.h>
using namespace std;

int main()
{
    string s1("hello ");
    string s2("world\n");
    struct iovec buff[2];
    buff[0].iov_base = const_cast<char *>(s1.c_str());
    buff[0].iov_len = s1.size();
    buff[1].iov_base = const_cast<char *>(s2.c_str());
    buff[1].iov_len = s2.size();
    writev(STDIN_FILENO, buff, 2);
    readv(STDOUT_FILENO, buff, 2);
    writev(STDIN_FILENO, buff, 2);
    return 0;
}
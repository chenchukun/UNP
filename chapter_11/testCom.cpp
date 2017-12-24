//
// Created by chenchukun on 17/12/24.
//

#include "sock.h"
#include "utility.h"
#include <getopt.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

void printAddrinfo(struct addrinfo *result, int index=0)
{
    string host;
    cout << "addrinfo[0] = " << endl;
    cout << "\tai_flags = " << result->ai_flags << endl;
    cout << "\tai_family = " << family_to_name(result->ai_family) << "(" << result->ai_family << ")" << endl;
    cout << "\tai_socktype = " << socktype_to_name(result->ai_socktype) << "(" << result->ai_socktype << ")" << endl;
    cout << "\tai_protocol = " << result->ai_protocol << endl;
    cout << "\tai_addrlen = " << result->ai_addrlen << endl;
    cout << "\tai_canonname = " << (result->ai_canonname==NULL?"":result->ai_canonname) << endl;
    if (sock_ntop(result->ai_addr, host) == NULL) {
        PrintStdError("sock_ntop:");
        exit(errno);
    }
    cout << "\tai_addr = " << host << endl;
    if (result->ai_next != NULL) {
        printAddrinfo(result->ai_next, index+1);
    }
}

void test_host_serv(int argc, char **argv)
{
    struct addrinfo *result;
    result = host_serv(argv[1], argv[2], AF_UNSPEC, SOCK_STREAM);
    if (result == NULL) {
        cout << gai_strerror(errno) << endl;
        exit(1);
    }
    printAddrinfo(result);
    freeaddrinfo(result);
}

void test_tcp_connect(int argc, char **argv)
{
    int sockfd = tcp_connect(argv[1], argv[2]);
    if (sockfd < 0) {
        PrintStdError("tcp_connect:");
        exit(errno);
    }
    cout << "connection success" << endl;
    close(sockfd);
}

void test_tcp_listen(int argc, char **argv)
{
    int listenfd = tcp_listen(argv[1], argv[2]);
    if (listenfd < 0) {
        PrintStdError("tcp_listen:");
        exit(errno);
    }
    cout << "listen success" << endl;
    int sockfd = accept(listenfd, NULL, NULL);
}

int main(int argc, char **argv)
{
    test_host_serv(argc, argv);
//    test_tcp_connect(argc, argv);
    test_tcp_listen(argc, argv);
    return 0;
}

//
// Created by chenchukun on 18/1/18.
//
#include <uv.h>
#include <iostream>
#include <thread>
#include <netdb.h>
#include "sock.h"
#include "utility.h"
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

void addrinfo_cb(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    if (status != 0) {
        cerr << "addrinfo_cb: " << uv_strerror(status) << endl;
        return;
    }
    printAddrinfo(res);
    uv_freeaddrinfo(res);
}

int main(int argc, char **argv)
{
    uv_loop_t *loop = uv_default_loop();

    // 异步getaddrinfo
    uv_getaddrinfo_t req;
    int ret = uv_getaddrinfo(loop, &req, addrinfo_cb, argv[1], argv[2], NULL);
    if (ret != 0) {
        cerr << "uv_getaddrinfo fail:" << uv_strerror(ret) << endl;
    }

    uv_run(loop, UV_RUN_DEFAULT);
    uv_stop(loop);
    return 0;
}

//
// Created by chenchukun on 17/11/28.
//

#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <iostream>
using namespace std;

int main()
{
    // 创建一个套接字文件描述符,成功返回非负整数,失败返回-1
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd >= 0) {
        cout << "创建套接字文件描述符成功" << endl;
    }
    else {
        cerr << "创建套接字文件描述符失败" << endl;
    }
    return 0;
}
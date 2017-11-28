//
// Created by chenchukun on 17/11/27.
//

#ifndef MAIN_UNP_H
#define MAIN_UNP_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>

// 将统配地址和一个临时端口绑定到一个套接字上
// 成功返回端口号,失败返回-1
int sock_bind_wild(int sockfd, int family);

// 将一个sockaddr地址转换为IP:PORT格式
const char* sock_ntop(const void *vaddr, char *host, int len);

// 创建一个协议地址,成功返回0,失败返回-1
int sockaddr_set(void *addr, int family, int16_t port, const char *ip=NULL);

// 创建一个监听套接字描述符
int sock_listen(int family, int backlog, int port, const std::string &ip="");

#endif //MAIN_UNP_H

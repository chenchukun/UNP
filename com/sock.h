//
// Created by chenchukun on 17/11/27.
//

#ifndef MAIN_UNP_H
#define MAIN_UNP_H

#include <netinet/in.h>
#include <string>

#define family_to_name(family) ((family)==AF_INET?"AF_INET": (family)==AF_INET6?"AF_INET6":"UNKNOWN")

/*
 * 读取n个字符,成功返回读取字符数,失败返回-1
 */
int readn(int fd, void *buff, size_t len);

/*
 * 写n个字符,成功返回写入字符数,失败返回-1
 */
int writen(int fd, const void *buff, size_t len);

int readline(int fd, void *buff, size_t len);

/*
 * 将统配地址和一个临时端口绑定到一个套接字上
 * 成功返回端口号,失败返回-1
 */
int sock_bind_wild(int sockfd, int family);

/*
 * 返回一个以绑定的sockfd,成功返回sockfd,失败返回-1
 * 成功返回0,失败返回-1
 */
int sock_bind(int sockfd, int family, uint16_t port=0, const char *ip=NULL);

/*
 * 将一个sockaddr地址转换为IP:PORT格式
 * 成功返回host,失败返回NULL
 */
const char* sock_ntop(const void *vaddr, char *host, int len);

const char* sock_ntop(const void *vaddr, std::string &host);

/*
 * 建一个sockaddr地址转为IP和PORT
 * 成功返回0,失败返回-1
 */
int sock_ntop(const void *vaddr, char *ip, int len, uint16_t &port);

int sock_ntop(const void *vaddr, std::string &ip, uint16_t &port);

/*
 * 创建一个协议地址,成功返回0,失败返回-1
 */
int sockaddr_set(void *addr, int family=AF_INET, uint16_t port=0, const char *ip=NULL);

/*
 * 创建一个监听套接字描述符,成功返回sockfd,失败返回-1
 */
int sock_listen(int family, int backlog, uint16_t port=0, const char *ip=NULL);

/*
 * 创建一个监听套接字描述符,成功返回sockfd,失败返回-1
 */
int sock_listen(int sockfd, int family, int backlog, uint16_t port=0, const char *ip=NULL);

/*
 * 返回一个连接套接字,成功返回connfd,失败返回-1
 */
int sock_connect(int family, uint16_t port, const char *ip);

/*
 * 获取套接字的地址族,成功返回地址族,失败返回-1
 */
int sockfd_to_family(int sockfd);

typedef void SigFunc(int);

SigFunc* signal_action(int signo, SigFunc *func);

#endif //MAIN_UNP_H

//
// Created by chenchukun on 17/11/27.
//

#ifndef MAIN_UNP_H
#define MAIN_UNP_H

// 将统配地址和一个临时端口绑定到一个套接字上
// 成功返回端口号,失败返回-1
int sock_bind_wild(int sockfd, int family);

#endif //MAIN_UNP_H

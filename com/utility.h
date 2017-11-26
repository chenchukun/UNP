//
// Created by chenchukun on 17/11/2.
//

#ifndef UNP_LIBARY_H
#define UNP_LIBARY_H
#include <unistd.h>
#include <sys/errno.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define CHECK_RETURN_EXIT(fun) \
    if (fun < 0) { \
        exit(-1); \
    }


int readn(int fd, void *buff, int len);

int writen(int fd, const void *buff, int len);

#endif //UNP_LIBARY_H

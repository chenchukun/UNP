//
// Created by chenchukun on 17/11/2.
//
#include "utility.h"

int readn(int fd, void *buff, int len)
{
    int offset = 0;
    int nread = 0;
    char *ptr = (char *) buff;
    while ((nread = read(fd, ptr+offset, len-offset)) >= 0) {
        if (nread == 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        offset += nread;
    }
    if (nread < 0) {
        return -1;
    }
    return offset;
}

int writen(int fd, const void *buff, int len)
{
    int nwrite = 0;
    int offset = 0;
    const char *ptr = (const char *) buff;
    while (offset<len && (nwrite = write(fd, ptr+offset, len-offset)) >= 0) {
        if (nwrite == 0) {
            if (errno == EINTR) {
                continue;
            }
            break;
        }
        offset += nwrite;
    }
    if (nwrite < 0) {
        return -1;
    }
    return offset;
}
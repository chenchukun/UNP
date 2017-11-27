//
// Created by chenchukun on 17/11/2.
//
#include "utility.h"

int readn(int fd, void *buff, size_t len)
{
    size_t offset = 0;
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

int writen(int fd, const void *buff, size_t len)
{
    int nwrite = 0;
    size_t offset = 0;
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

int readline(int fd, void *buff, size_t len)
{
    size_t nread = len;
    char *ptr = reinterpret_cast<char*>(buff);
    for (ssize_t i=0; i<len-1; ++i) {
        int ret = read(fd, ptr+i, 1);
        if (ret == 1) {
            if (*(ptr + i) == '\n') {
                if (i - 1 >= 0 && *(ptr + i - 1)=='\r') {
                    *(ptr + i - 1) = 0;
                    return i-1;
                }
                *(ptr + i) = 0;
                return i;
            }
        }
        else if (ret == 0) {
            *(ptr+1) = 0;
            return i;
        }
        else if (errno == EINTR) {
            --i;
            continue;
        }
        return -1;
    }
    *(ptr + len-1) = 0;
    return len - 1;
}
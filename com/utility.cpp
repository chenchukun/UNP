//
// Created by chenchukun on 17/11/2.
//
#include "utility.h"
#include <memory>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/errno.h>
#include <cstdio>

int readn(int fd, void *buff, size_t len)
{
    size_t offset = 0;
    int nread = 0;
    char *ptr = (char *) buff;
    while (true) {
        nread = read(fd, ptr+offset, len-offset);
        if (nread == 0) {
            break;
        }
        if (nread > 0) {
            offset += nread;
            continue;
        }
        if (errno == EINTR) {
            continue;
        }
        return -1;
    }
    return offset;
}

int writen(int fd, const void *buff, size_t len)
{
    int nwrite = 0;
    size_t offset = 0;
    const char *ptr = (const char *) buff;
    while (true) {
        nwrite = write(fd, ptr+offset, len-offset);
        // write()什么时候返回0???
        if (nwrite == 0) {
            break;
        }
        if (nwrite > 0) {
            offset += nwrite;
            continue;
        }
        if (errno == EINTR) {
            continue;
        }
        return -1;
    }
    return offset;
}

int readline(int fd, void *buff, size_t len)
{
    size_t nread = len;
    char *ptr = reinterpret_cast<char*>(buff);
    for (size_t i=0; i<len-1; ++i) {
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
            *(ptr+i) = 0;
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

std::string& strTrim(std::string &str)
{
    while (str.size() > 0) {
        if (isblank(*str.begin())) {
            str.erase(str.begin());
            continue;
        }
        break;
    }
    while (str.size() > 0) {
        if (isblank(*(str.end()-1))) {
            str.erase(str.end()-1);
            continue;
        }
        break;
    }
    return str;
}

std::vector<std::string> strSplit(const std::string &str, const std::string &sep)
{
    std::vector<std::string> result;
    size_t pos = 0;
    std::string::size_type index = 0;
    while ((index = str.find_first_of(sep, pos)) != std::string::npos) {
        result.push_back(std::string(str.cbegin()+pos, str.cbegin()+index));
        pos = index + sep.size();
    }
    if (pos < str.size()) {
        result.push_back(std::string(str.cbegin()+pos, str.cend()));
    }
    return result;
}
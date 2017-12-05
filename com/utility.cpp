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
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <iostream>

void PrintStdError(const std::string &msg)
{
    std::cerr << msg << ":" << strerror(errno) << "(" << errno << ")" << std::endl;
}

std::string& StrTrim(std::string &str)
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

std::vector<std::string> StrSplit(const std::string &str, const std::string &sep)
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

void Sleep(long usec)
{
    timeval tv;
    tv.tv_sec = usec / 1000000;
    tv.tv_usec = usec % 1000000;
    int ret;
    do {
        ret = select(0, NULL, NULL, NULL, &tv);
    } while (ret == -1 && errno == EINTR);
}
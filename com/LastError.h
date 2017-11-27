//
// Created by chenchukun on 17/11/26.
//

#ifndef UNP_LASTERROR_H
#define UNP_LASTERROR_H

#include <string>

class LastError
{
public:
    LastError(): _errno(0), _error("") {}

    void setLastError(int no, const std::string &str) {
        _errno = no;
        _error = str;
    }

    void setLastErrno(int no) {
        _errno = no;
    }

    void setLastError(const std::string &str) {
        _error = str;
    }

    const char* getLastError() {
        return _error.c_str();
    }

    int getLastErrno() {
        return _errno;
    }

private:
    int _errno;

    std::string _error;
};

#endif //UNP_LASTERROR_H

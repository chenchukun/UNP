//
// Created by chenchukun on 17/11/2.
//

#ifndef UNP_LIBARY_H
#define UNP_LIBARY_H
#include <string>
#include <vector>
#include <sstream>

#define CHECK_RETURN_EXIT(fun) \
    if (fun < 0) { \
        exit(-1); \
    }

/*
 * 去除字符串首尾的空白字符
 */
std::string& StrTrim(std::string &str);

/*
 * 分割字符串
 */
std::vector<std::string> StrSplit(const std::string &str, const std::string &sep);

template<typename It>
std::string VectorToStr(It first, It last)
{
    if (first == last) {
        return "[]";
    }
    std::string result;
    std::stringstream stream;
    stream << "[";
    while (first != last) {
        stream << *first << ", ";
        ++first;
    }
    result = stream.str();
    result.erase(result.end()-2, result.end());
    result += "]";
    return result;
}

void Sleep(long usec);

#endif //UNP_LIBARY_H

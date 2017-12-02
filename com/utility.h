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
 * 读取n个字符,成功返回读取字符数,失败返回-1
 */
int readn(int fd, void *buff, size_t len);

/*
 * 写n个字符,成功返回写入字符数,失败返回-1
 */
int writen(int fd, const void *buff, size_t len);

/*
 * 去除字符串首尾的空白字符
 */
std::string& strTrim(std::string &str);

/*
 * 分割字符串
 */
std::vector<std::string> strSplit(const std::string &str, const std::string &sep);

template<typename It>
std::string vectorToStr(It first, It last)
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


#endif //UNP_LIBARY_H

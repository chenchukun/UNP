//
// Created by chenchukun on 18/2/9.
//

#ifndef MAIN_COMMON_H
#define MAIN_COMMON_H

#include <execinfo.h>
#include <ctime>
#include <sys/time.h>
#include <string>

#define CHECK_ZERO_RETURN(fun)  \
    {                           \
        int check_ret = fun;        \
        if (check_ret != 0) {         \
            return check_ret;         \
        }                       \
    }


inline std::string NOW_TIME()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t t = tv.tv_sec;
    struct tm ptm;
    localtime_r(&t, &ptm);
    std::string buff(24, 0);
    sprintf(const_cast<char*>(buff.data()), "%04d-%02d-%02d %02d:%02d:%02d.%03d", ptm.tm_year + 1900,
    ptm.tm_mon+1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec, int(tv.tv_usec/1000));
    return buff;
}


#define LOG_DEBUG(fmt, ...) \
    printf("DEBUG %s %s[line:%d] " fmt "\n", NOW_TIME().c_str(), __FILE__, __LINE__, ##__VA_ARGS__);

#define LOG_INFO(fmt, ...) \
    printf("INFO %s %s[line:%d] " fmt "\n", NOW_TIME().c_str(), __FILE__, __LINE__, ##__VA_ARGS__);

#define LOG_WARN(fmt, ...) \
    printf("WARN %s %s[line:%d] " fmt "\n", NOW_TIME().c_str(), __FILE__, __LINE__, ##__VA_ARGS__);

#define LOG_ERROR(fmt, ...) \
    printf("ERROR %s %s[line:%d] " fmt "\n", NOW_TIME().c_str(), __FILE__, __LINE__, ##__VA_ARGS__);


#ifdef __linux__

#define htonll(val) ((((int64_t) htonl(val)) << 32) + htonl(val >> 32))

#define ntohll(val) ((((int64_t) ntohl(val)) << 32) + ntohl(val >> 32))

#endif


#define printStacktrace() {                                    \
    int size = 16;                                              \
    void * array[16];                                           \
    int stack_num = backtrace(array, size);                     \
    char ** stacktrace = backtrace_symbols(array, stack_num);   \
    for (int i = 0; i < stack_num; ++i)                         \
    {                                                           \
        printf("%s\n", stacktrace[i]);                          \
    }                                                           \
    free(stacktrace);                                           \
}

#define NAMESPACE_START namespace uv_cpp {

#define NAMESPACE_END }

#endif //MAIN_COMMON_H

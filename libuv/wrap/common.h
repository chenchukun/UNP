//
// Created by chenchukun on 18/2/9.
//

#ifndef MAIN_COMMON_H
#define MAIN_COMMON_H

#ifdef _WIN32

#include <WinSock2.h>
#include <winsock.h>
#include <windows.h>

#else

#include <execinfo.h>
#include <sys/time.h>

#endif
#include <ctime>
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
#ifdef _WIN32
    SYSTEMTIME sys;
	GetLocalTime(&sys);
	std::string buff(24, 0);
	sprintf(const_cast<char*>(buff.data()), "%4d/%02d/%02d %02d:%02d:%02d.%03d",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	return buff;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t t = tv.tv_sec;
    struct tm ptm;
    localtime_r(&t, &ptm);
    std::string buff(24, 0);
    sprintf(const_cast<char*>(buff.data()), "%04d-%02d-%02d %02d:%02d:%02d.%03d", ptm.tm_year + 1900,
    ptm.tm_mon+1, ptm.tm_mday, ptm.tm_hour, ptm.tm_min, ptm.tm_sec, int(tv.tv_usec/1000));
    return buff;
#endif
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

#ifndef _WIN32
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
#endif

#ifdef _WIN32
inline int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif

#define NAMESPACE_START namespace uv_cpp {

#define NAMESPACE_END }

#endif //MAIN_COMMON_H

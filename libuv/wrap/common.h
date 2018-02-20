//
// Created by chenchukun on 18/2/9.
//

#ifndef MAIN_COMMON_H
#define MAIN_COMMON_H

#include <execinfo.h>

#define CHECK_ZERO_RETURN(fun)  \
    {                           \
        int check_ret = fun;        \
        if (check_ret != 0) {         \
            return check_ret;         \
        }                       \
    }


#define LOG(fmt, ...) \
    printf("[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

/*
#define htonll(val) {       \
    return (((int64_t) htonl(val)) << 32) + htonl(val >> 32);   \
}

#define ntohll(val) {       \
    return (((int64_t) ntohl(val)) << 32) + ntohl(val >> 32);   \
}
*/


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

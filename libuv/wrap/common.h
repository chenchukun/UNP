//
// Created by chenchukun on 18/2/9.
//

#ifndef MAIN_COMMON_H
#define MAIN_COMMON_H

#define CHECK_ZERO_RETURN(fun)  \
    {                           \
        int check_ret = fun;        \
        if (check_ret != 0) {         \
            return check_ret;         \
        }                       \
    }

#endif //MAIN_COMMON_H

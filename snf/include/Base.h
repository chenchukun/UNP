#ifndef BASE_H
#include <sys/types.h>

namespace snf 
{

ssize_t readn(int fd, void *buff, size_t len);

ssize_t writen(int fd, const void *buff, size_t len);

ssize_t readline(int fd, void *buff, size_t maxlen);

}

#endif

//
// Created by chenchukun on 17/11/1.
//
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <ctime>
#include <unistd.h>

int main()
{
    int lisFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6180);

    bind(lisFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    listen(lisFd, 100);
    char buff[64];
    while (true) {
        struct sockaddr_in remoteAddr;
        socklen_t len;
        int sock = accept(lisFd, reinterpret_cast<sockaddr*>(&remoteAddr), &len);
        if (sock >= 0) {
            time_t now = time(NULL);
            ctime_r(&now, buff);
            write(sock, buff, strlen(buff));
            close(sock);
        }
    }
    return 0;
}

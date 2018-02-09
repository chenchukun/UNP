//
// Created by chenchukun on 18/2/9.
//

#ifndef MAIN_TCPSERVER_HPP
#define MAIN_TCPSERVER_HPP

#include <uv.h>
#include <cstdlib>
#include <string>

class TcpServer
{
public:
    TcpServer(uv_loop_t *loop)
        : loop_(loop),
          connectionCb_(NULL)
    {

    }

    int start(const std::string &ip, int port, int backlog=1024);

    int start(int port, int backlog=1024);

    void setConnectionCallback(uv_connection_cb cb) {
        connectionCb_ = cb;
    }

private:
    uv_loop_t *loop_;

    uv_tcp_t *server_;

    uv_connection_cb connectionCb_;
};

#endif //MAIN_TCPSERVER_HPP

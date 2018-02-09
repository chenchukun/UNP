//
// Created by chenchukun on 18/2/9.
//

#include "TcpServer.h"
#include "common.h"

int TcpServer::start(const std::string &ip, int port, int backlog) {

    server_ = static_cast<uv_tcp_t*>(malloc(sizeof(uv_tcp_t)));

    CHECK_ZERO_RETURN(uv_tcp_init(loop_, server_));

    struct sockaddr_in addr;

    CHECK_ZERO_RETURN(uv_ip4_addr(ip.c_str(), port, &addr));
    CHECK_ZERO_RETURN(uv_tcp_bind(server_, reinterpret_cast<struct sockaddr*>(&addr), 0));

    CHECK_ZERO_RETURN(uv_listen(reinterpret_cast<uv_stream_t*>(server_), backlog, connectionCb_));

    return 0;
}

int TcpServer::start(int port, int backlog) {
    return start("0.0.0.0", port);
}
//
// Created by chenchukun on 18/2/9.
//

#include "TcpServer.h"
#include "common.h"
#include <iostream>
using namespace std;

int TcpServer::start(const std::string &ip, int port, int backlog) {

    server_ = static_cast<uv_tcp_t*>(malloc(sizeof(uv_tcp_t)));

    server_->data = this;

    CHECK_ZERO_RETURN(uv_tcp_init(loop_, server_));

    struct sockaddr_in addr;

    CHECK_ZERO_RETURN(uv_ip4_addr(ip.c_str(), port, &addr));
    CHECK_ZERO_RETURN(uv_tcp_bind(server_, reinterpret_cast<struct sockaddr*>(&addr), 0));

    CHECK_ZERO_RETURN(uv_listen(reinterpret_cast<uv_stream_t*>(server_), backlog, TcpServer::connectionCallback));

    return 0;
}

int TcpServer::start(int port, int backlog) {
    return start("0.0.0.0", port);
}

// ========== static callback ============

void TcpServer::connectionCallback(uv_stream_t *server, int status)
{
    for (int i=0; i<3; ++i) {
        TcpConnectionPtr connectionPtr = make_shared<TcpConnection>(server->loop);
        int ret = uv_accept(server, reinterpret_cast<uv_stream_t*>(&connectionPtr->client_));
        if (ret == 0) {
            TcpServer *server_ = static_cast<TcpServer*>(server->data);
            connectionPtr->client_.data = server_;
            {
                // 在IO线程执行
 //               lock_guard<mutex> guard(server_->mutex_);
                server_->connectionMap_[reinterpret_cast<int64_t>(&connectionPtr->client_)] = connectionPtr;
            }
            ret = uv_read_start(reinterpret_cast<uv_stream_t*>(&connectionPtr->client_),
                                TcpConnection::allocCallback, TcpConnection::readCallback);
            if (ret != 0) {
                connectionPtr->shutdown();
                cerr << "uv_read_start: " << uv_strerror(ret) << endl;
                // 错误回调?
            }
            if (server_->connectionCallback_ != NULL) {
                server_->connectionCallback_(connectionPtr);
            }
        }
        else if (ret == -35) {
            break;
        }
        else {
            // 错误回调?
            cerr << "uv_accept: " << strerror(ret) << "(" << ret << ")" << endl;
        }
    }
}


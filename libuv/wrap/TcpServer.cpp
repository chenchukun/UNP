//
// Created by chenchukun on 18/2/9.
//

#include "TcpServer.h"
#include "common.h"
#include "EventLoop.h"
#include <iostream>
using namespace std;

NAMESPACE_START

TcpServer::TcpServer(EventLoop *eventLoop)
    : eventLoop_(eventLoop),
      loop_(eventLoop_->getLoop()),
      connectionCallback_(NULL),
      messageCallback_(NULL),
      errorCallback_(NULL),
      autoId_(0)
{
}

int TcpServer::start(const SockAddr &addr, int backlog) {

    server_ = static_cast<uv_tcp_t*>(malloc(sizeof(uv_tcp_t)));

    server_->data = this;

    CHECK_ZERO_RETURN(uv_tcp_init(loop_, server_));

    CHECK_ZERO_RETURN(uv_tcp_bind(server_, addr.getAddr(), 0));

    CHECK_ZERO_RETURN(uv_listen(reinterpret_cast<uv_stream_t*>(server_), backlog, TcpServer::connectionCallback));

    return 0;
}

// ========== static callback ============

void TcpServer::connectionCallback(uv_stream_t *server, int status)
{
    if (status != 0) {
        LOG_ERROR("connectionCallback: %s(%s)", uv_strerror(status), uv_err_name(status));
        return;
    }
    TcpServer *server_ = static_cast<TcpServer*>(server->data);
    for (int i=0; i<3; ++i) {
        TcpConnectionPtr connectionPtr = make_shared<TcpConnection>(server_, server_->autoId_++);
        connectionPtr->client_.data = static_cast<void*>(connectionPtr.get());
        int ret = uv_accept(server, reinterpret_cast<uv_stream_t*>(&connectionPtr->client_));
        if (ret == 0) {
            {
                // 在IO线程执行
 //               lock_guard<mutex> guard(server_->mutex_);
                server_->connectionMap_[connectionPtr->id_] = connectionPtr;
            }
            ret = uv_read_start(reinterpret_cast<uv_stream_t*>(&connectionPtr->client_),
                                TcpConnection::allocCallback, TcpConnection::readCallback);
            if (ret != 0) {
                connectionPtr->shutdown();
                LOG_ERROR("uv_read_start: %s(%s)", uv_strerror(ret), uv_err_name(ret));
                // 错误回调?
            }
            if (server_->connectionCallback_ != NULL) {
                server_->connectionCallback_(connectionPtr);
            }
        }
        else if (ret == UV_EAGAIN) {
            break;
        }
        else {
            // 错误回调?
            LOG_ERROR("uv_accept: %s(%s)", uv_strerror(ret), uv_err_name(ret));
        }
    }
}

NAMESPACE_END
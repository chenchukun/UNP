//
// Created by chenchukun on 18/2/11.
//

#include "TcpConnection.h"
#include "TcpServer.h"
#include <iostream>
#include <vector>
using namespace std;

NAMESPACE_START

TcpConnection::TcpConnection(TcpServer *server, uint64_t id)
    : server_(server),
      remoteClose_(false),
      id_(id)
{
    uv_tcp_init(server_->loop_, &client_);
}

bool TcpConnection::connected()
{
    return !remoteClose_;
}

shared_ptr<SockAddr> TcpConnection::getLocalAddr()
{
    std::shared_ptr<SockAddr> paddr = make_shared<SockAddr>();
    int len = paddr->getAddrLength();
    int ret = uv_tcp_getsockname(&client_, paddr->getAddr(), &len);
    if (ret != 0) {
        return NULL;
    }
    return paddr;
}

shared_ptr<SockAddr> TcpConnection::getRemoteAddr()
{
    std::shared_ptr<SockAddr> paddr = make_shared<SockAddr>();
    int len = paddr->getAddrLength();
    int ret = uv_tcp_getpeername(&client_, paddr->getAddr(), &len);
    if (ret != 0) {
        return NULL;
    }
    return paddr;
}

void TcpConnection::shutdown()
{
    uv_shutdown_t *req = static_cast<uv_shutdown_t*>(malloc(sizeof(uv_shutdown_t)));
    req->data = static_cast<void*>(this);
    uv_shutdown(req, reinterpret_cast<uv_stream_t*>(&client_), TcpConnection::shutdownCallback);
}

// ========== static callback ============

void TcpConnection::closeCallback(uv_handle_t* handle)
{
    TcpConnection *conn = static_cast<TcpConnection*>(handle->data);
    TcpServer *server = conn->server_;
    {
        // uv_close回调是在IO线程执行?
//        lock_guard<mutex> guard(server_->mutex_);
        server->connectionMap_.erase(conn->id_);
    }
}

void TcpConnection::shutdownCallback(uv_shutdown_t* handle, int status)
{
    TcpConnection *conn = static_cast<TcpConnection*>(handle->data);
    TcpServer *server = conn->server_;
    uv_close(reinterpret_cast<uv_handle_t*>(&conn->client_), TcpConnection::closeCallback);
}

void TcpConnection::readCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    TcpConnection *conn = static_cast<TcpConnection*>(stream->data);
    TcpServer *server = conn->server_;
    auto it = server->connectionMap_.find(conn->id_);
    assert(it != server->connectionMap_.end());

    if (it != server->connectionMap_.end()) {
        // 对端关闭连接
        if (nread == UV_EOF) {
            it->second->remoteClose_ = true;
            server->connectionCallback_(it->second);
        }
        else if (nread > 0){
            it->second->readBuff_.setWritePosition(it->second->readBuff_.writePos_ + nread);
            if (server->messageCallback_ != NULL) {
                server->messageCallback_(it->second, it->second->readBuff_);
            }
            else {
                // 丢弃信息
                it->second->readBuff_.clear();
            }
        }
        else {
            if (server->errorCallback_ != NULL) {
                server->errorCallback_(it->second, nread);
            }
        }
    }
    else {
        // BUG?
    }
}

void TcpConnection::allocCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    TcpConnection *conn = static_cast<TcpConnection*>(handle->data);
    TcpServer *server = conn->server_;
    auto it = server->connectionMap_.find(conn->id_);
    assert(it != server->connectionMap_.end());

    if (it != server->connectionMap_.end()) {
        TcpConnectionPtr connectionPtr = it->second;
        connectionPtr->readBuff_.initUVBuffer(buf);
    }
    else {
        // BUG?
    }
}

void TcpConnection::writeComplete(uv_write_t* req, int status)
{
    pair<string*, uv_buf_t*> *data = static_cast<pair<string*, uv_buf_t*>*>(req->data);
    delete data->first;
    delete data->second;
}

void TcpConnection::send(const string &str)
{
    uv_write_t *handle = static_cast<uv_write_t*>(malloc(sizeof(uv_write_t)));
    string *data = new string;
    *data = move(str);
    uv_buf_t *buf = static_cast<uv_buf_t*>(malloc(sizeof(uv_buf_t)));
    buf->base = const_cast<char*>(data->data());
    buf->len = data->size();
    handle->data = static_cast<void*>(new pair<string*, uv_buf_t*>(data, buf));
    uv_write(handle, reinterpret_cast<uv_stream_t*>(&client_), buf, 1, TcpConnection::writeComplete);
}

NAMESPACE_END
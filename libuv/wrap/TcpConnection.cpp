//
// Created by chenchukun on 18/2/11.
//

#include "TcpConnection.h"
#include "TcpServer.h"
#include "EventLoop.h"
#include <iostream>
#include <vector>
using namespace std;

NAMESPACE_START

TcpConnection::TcpConnection(TcpServer *server, uint64_t id)
    : server_(server),
      status_(CONNECTED),
      id_(id)
{
    client_ = static_cast<uv_tcp_t*>(malloc(sizeof(uv_tcp_t)));
    uv_tcp_init(server_->loop_, client_);
}

bool TcpConnection::connected()
{
    return status_ == CONNECTED;
}

shared_ptr<SockAddr> TcpConnection::getLocalAddr()
{
    std::shared_ptr<SockAddr> paddr = make_shared<SockAddr>();
    int len = paddr->getAddrLength();
    int ret = uv_tcp_getsockname(client_, paddr->getAddr(), &len);
    if (ret != 0) {
        return NULL;
    }
    return paddr;
}

shared_ptr<SockAddr> TcpConnection::getPeerAddr()
{
    std::shared_ptr<SockAddr> paddr = make_shared<SockAddr>();
    int len = paddr->getAddrLength();
    int ret = uv_tcp_getpeername(client_, paddr->getAddr(), &len);
    if (ret != 0) {
        return NULL;
    }
    return paddr;
}

void TcpConnection::close()
{

}

void TcpConnection::shutdown()
{
    uv_shutdown_t *req = static_cast<uv_shutdown_t*>(malloc(sizeof(uv_shutdown_t)));
    req->data = static_cast<void*>(this);
    uv_tcp_t *client = client_;
    server_->eventLoop_->runInLoopThread([client, req] {
        uv_shutdown(req, reinterpret_cast<uv_stream_t*>(client), TcpConnection::shutdownCallback);
    });
}

EventLoop* TcpConnection::getEventLoop()
{
    return server_->eventLoop_;
}

// ========== static callback ============

void TcpConnection::closeCallback(uv_handle_t* handle)
{
    weak_ptr<TcpConnection> *weak = static_cast<weak_ptr<TcpConnection>*>(handle->data);
    TcpConnectionPtr conn = weak->lock();
    assert(conn != NULL);
    TcpServer *server = conn->server_;
    {
        // uv_close回调是在IO线程执行?
//        lock_guard<mutex> guard(server_->mutex_);
        server->connectionMap_.erase(conn->id_);
    }
    delete weak;
    free(handle);
}

void TcpConnection::shutdownCallback(uv_shutdown_t* handle, int status)
{
    TcpConnection *conn = static_cast<TcpConnection*>(handle->data);
    uv_close(reinterpret_cast<uv_handle_t*>(conn->client_), TcpConnection::closeCallback);
    free(handle);
}

void TcpConnection::readCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    TcpConnectionPtr conn = static_cast<weak_ptr<TcpConnection>*>(stream->data)->lock();
    assert(conn != NULL);
    TcpServer *server = conn->server_;
    // 对端关闭连接
    if (nread == UV_EOF) {
        conn->status_ = CLOSE_WAIT;
        server->connectionCallback_(conn);
    }
    else if (nread > 0){
        conn->readBuff_.setWritePosition(conn->readBuff_.writePos_ + nread);
        if (server->messageCallback_ != NULL) {
            server->messageCallback_(conn, conn->readBuff_);
        }
        else {
            // 丢弃信息
            conn->readBuff_.clear();
        }
    }
    else {
        if (server->errorCallback_ != NULL) {
            server->errorCallback_(conn, nread);
        }
    }
}

void TcpConnection::allocCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    TcpConnectionPtr conn = static_cast<weak_ptr<TcpConnection>*>(handle->data)->lock();
    assert(conn != NULL);
    conn->readBuff_.initUVBuffer(buf);
}

void TcpConnection::writeComplete(uv_write_t* req, int status)
{
    WriteContext *context = static_cast<WriteContext*>(req->data);
    if (context->conn->server_->writeCompleteCallback_ != NULL) {
        context->conn->server_->writeCompleteCallback_(context->conn);
    }
    switch (context->buffType) {
        case BUF_TYPE_STRING:
            delete context->str;
            break;
    }
    free(context->buf);
    delete context;
    free(req);
}

void TcpConnection::send(const string &str)
{
    uv_tcp_t *client = client_;
    TcpConnectionPtr tcpConnectionPtr = shared_from_this();
    uv_write_t *handle = static_cast<uv_write_t*>(malloc(sizeof(uv_write_t)));
    string *data = new string;
    *data = move(str);
    uv_buf_t *buf = static_cast<uv_buf_t*>(malloc(sizeof(uv_buf_t)));
    buf->base = const_cast<char*>(data->data());
    buf->len = data->size();
    WriteContext *context = new WriteContext;
    context->buffType = BUF_TYPE_STRING;
    context->buf = buf;
    context->str = data;
    context->conn = tcpConnectionPtr;
    handle->data = static_cast<void*>(context);
    server_->eventLoop_->runInLoopThread([client, buf, handle] {
        uv_write(handle, reinterpret_cast<uv_stream_t*>(client), buf, 1, TcpConnection::writeComplete);
    });
}

NAMESPACE_END
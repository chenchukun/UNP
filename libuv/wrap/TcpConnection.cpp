//
// Created by chenchukun on 18/2/11.
//

#include "TcpConnection.h"
#include "TcpServer.h"
#include <iostream>
#include <vector>
using namespace std;

bool TcpConnection::connected()
{
    return !remoteClose_;
}

std::shared_ptr<sockaddr_in> TcpConnection::getLocalAddr()
{
    std::shared_ptr<sockaddr_in> paddr = make_shared<sockaddr_in>();
    int len = sizeof(*paddr);
    int ret = uv_tcp_getsockname(&client_, (sockaddr*)paddr.get(), &len);
    if (ret != 0) {
        return NULL;
    }
    return paddr;
}

std::shared_ptr<sockaddr_in> TcpConnection::getRemoteAddr()
{
    std::shared_ptr<sockaddr_in> paddr = make_shared<sockaddr_in>();
    int len = sizeof(*paddr);
    int ret = uv_tcp_getpeername(&client_, (sockaddr*)paddr.get(), &len);
    if (ret != 0) {
        return NULL;
    }
    return paddr;
}

void TcpConnection::shutdown()
{
    uv_close(reinterpret_cast<uv_handle_t*>(&client_), TcpConnection::shutdownCallback);
}

// ========== static callback ============

void TcpConnection::shutdownCallback(uv_handle_t* handle)
{
    TcpServer *server_ = static_cast<TcpServer*>(handle->data);
    {
        // uv_close回调是在IO线程执行?
//        lock_guard<mutex> guard(server_->mutex_);
        server_->connectionMap_.erase(reinterpret_cast<int64_t>(handle));
    }
}

void TcpConnection::readCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    TcpServer *server = static_cast<TcpServer*>(stream->data);
    auto it = server->connectionMap_.find(reinterpret_cast<int64_t>(stream));
    if (it != server->connectionMap_.end()) {
        // 对端关闭连接
        if (nread == UV_EOF) {
            it->second->remoteClose_ = true;
            server->connectionCallback_(it->second);
        }
        else if (nread > 0){
            if (server->messageCallback_ != NULL) {
                it->second->readBuff_.writePos_ += nread;
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
    TcpServer *server = static_cast<TcpServer*>(handle->data);
    auto it = server->connectionMap_.find(reinterpret_cast<int64_t>(handle));
    if (it != server->connectionMap_.end()) {
        TcpConnectionPtr connectionPtr = it->second;
        connectionPtr->readBuff_.initUVReadBuf(buf);
    }
    else {
        // BUG?
    }
}

void TcpConnection::writeComplete(uv_write_t* req, int status)
{
    pair<bool*, void*> *data = static_cast<pair<bool*, void*>*>(req->data);
    vector<char*> *backup = static_cast<vector<char*>*>(data->second);
    for (auto item : *backup) {
        free(item);
    }
    free(backup);
    free(data);
    *(data->first) = false;
}

void TcpConnection::send(const std::string &str)
{
    lock_guard<mutex> guard(mutex_);
    writeBuff_.append(str);
    if (sending_ == false) {
        sendImpl();
    }
}

void TcpConnection::sendImpl()
{
    sending_ = true;
    uv_buf_t *buf;
    size_t size = writeBuff_.initUVWriteBuf(&writeReq, &buf);
    writeReq.data = new pair<bool*, void*>(&sending_, writeReq.data);
    uv_write(reinterpret_cast<uv_write_t*>(&writeReq), reinterpret_cast<uv_stream_t*>(&client_),
             buf, size, TcpConnection::writeComplete);
}

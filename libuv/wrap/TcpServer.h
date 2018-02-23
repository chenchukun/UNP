//
// Created by chenchukun on 18/2/9.
//

#ifndef MAIN_TCPSERVER_HPP
#define MAIN_TCPSERVER_HPP

#include <uv.h>
#include <cstdlib>
#include <map>
#include <mutex>
#include "common.h"
#include <string>
#include "SockAddr.h"
#include "TcpConnection.h"

NAMESPACE_START

typedef void (*ConnectionCallback)(TcpConnectionPtr &connectionPtr);

typedef void (*MessageCallback)(TcpConnectionPtr &connectionPtr, Buffer &buffer);

typedef void (*ErrorCallback)(TcpConnectionPtr &connectionPtr, int errcode);

typedef void (*WriteCompleteCallback)(TcpConnectionPtr &connectionPtr);

class EventLoop;

class TcpServer
{
public:
    friend class TcpConnection;

    TcpServer(EventLoop *eventLoop);

    ~TcpServer() {
        uv_close(reinterpret_cast<uv_handle_t*>(server_), NULL);
        free(server_);
    }

    int start(const SockAddr &addr, int backlog=1024);

    void setConnectionCallback(ConnectionCallback cb) {
        connectionCallback_ = cb;
    }

    void setMessageCallback(MessageCallback messageCallback) {
        messageCallback_ = messageCallback;
    }

    void setErrorCallback(ErrorCallback errorCallback) {
        errorCallback_ = errorCallback;
    }

    void setWriteCompleteCallback(WriteCompleteCallback writeCompleteCallback) {
        writeCompleteCallback_ = writeCompleteCallback;
    }

private:
    static void connectionCallback(uv_stream_t* server, int status);

    static void closeCallback(uv_handle_t* handle);

private:

    EventLoop *eventLoop_;

    uv_loop_t *loop_;

    uv_tcp_t *server_;

    mutable std::mutex mutex_;

    std::map<int, TcpConnectionPtr> connectionMap_;

    ConnectionCallback connectionCallback_;

    MessageCallback messageCallback_;

    ErrorCallback errorCallback_;

    WriteCompleteCallback writeCompleteCallback_;

    uint64_t autoId_;
};

NAMESPACE_END

#endif //MAIN_TCPSERVER_HPP

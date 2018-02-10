//
// Created by chenchukun on 18/2/9.
//

#ifndef MAIN_TCPSERVER_HPP
#define MAIN_TCPSERVER_HPP

#include <uv.h>
#include <cstdlib>
#include <map>
#include <mutex>
#include <string>
#include "TcpConnection.h"

typedef void (*ConnectionCallback)(TcpConnectionPtr &connectionPtr);

typedef void (*MessageCallback)(TcpConnectionPtr &connectionPtr, Buffer &buffer);

typedef void (*ErrorCallback)(TcpConnectionPtr &connectionPtr, int errcode);

class TcpServer
{
public:
    friend TcpConnection;

    TcpServer(uv_loop_t *loop)
        : loop_(loop),
          connectionCallback_(NULL),
          messageCallback_(NULL),
          errorCallback_(NULL)
    {

    }

    int start(const std::string &ip, int port, int backlog=1024);

    int start(int port, int backlog=1024);

    void setConnectionCallback(ConnectionCallback cb) {
        connectionCallback_ = cb;
    }

    void setMessageCallback(MessageCallback messageCallback) {
        messageCallback_ = messageCallback;
    }

    void setErrorCallback(ErrorCallback errorCallback) {
        errorCallback_ = errorCallback;
    }

private:
    static void connectionCallback(uv_stream_t* server, int status);

private:
    uv_loop_t *loop_;

    uv_tcp_t *server_;

    mutable std::mutex mutex_;

    std::map<int, TcpConnectionPtr> connectionMap_;

    ConnectionCallback connectionCallback_;

    MessageCallback messageCallback_;

    ErrorCallback errorCallback_;
};

#endif //MAIN_TCPSERVER_HPP

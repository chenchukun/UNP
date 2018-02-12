//
// Created by chenchukun on 18/2/11.
//

#ifndef MAIN_TCPCONNECTION_H
#define MAIN_TCPCONNECTION_H

#include <uv.h>
#include <memory>
#include <mutex>
#include "Buffer.h"
#include <uv.h>

class TcpServer;

class TcpConnection
{
public:
    friend class TcpServer;

    TcpConnection(uv_loop_t *loop)
        : loop_(loop),
          remoteClose_(false),
          sending_(false)
    {
        uv_tcp_init(loop_, &client_);
    }

    bool connected();

    std::shared_ptr<sockaddr_in> getLocalAddr();

    std::shared_ptr<sockaddr_in> getRemoteAddr();

    void shutdown();

    void send(const std::string &str);

private:
    static void shutdownCallback(uv_handle_t* handle);

    static void readCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

    static void allocCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

    static void writeComplete(uv_write_t* req, int status);

    void sendImpl();

private:
    uv_tcp_t client_;

    uv_loop_t *loop_;

    bool remoteClose_;

    bool sending_;

    std::mutex mutex_;

    Buffer readBuff_, writeBuff_;

    uv_write_t writeReq;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

#endif //MAIN_TCPCONNECTION_H

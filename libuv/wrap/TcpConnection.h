//
// Created by chenchukun on 18/2/11.
//

#ifndef MAIN_TCPCONNECTION_H
#define MAIN_TCPCONNECTION_H

#include <uv.h>
#include <memory>
#include <mutex>
#include "common.h"
#include "Buffer.h"
#include "SockAddr.h"
#include <uv.h>

NAMESPACE_START

class TcpServer;

class TcpConnection
{
public:
    friend class TcpServer;

    TcpConnection(TcpServer *server, uint64_t id);

    bool connected();

    std::shared_ptr<SockAddr> getLocalAddr();

    std::shared_ptr<SockAddr> getRemoteAddr();

    void shutdown();

    void send(const std::string &str);

private:
    static void shutdownCallback(uv_shutdown_t* handle, int status);

    static void readCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

    static void allocCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

    static void writeComplete(uv_write_t* req, int status);

    static void closeCallback(uv_handle_t* handle);


private:
    uv_tcp_t *client_;

    TcpServer *server_;

    bool remoteClose_;

    std::mutex mutex_;

    Buffer readBuff_;

    uint64_t id_;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

NAMESPACE_END

#endif //MAIN_TCPCONNECTION_H

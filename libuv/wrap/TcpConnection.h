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
#include "EventLoop.h"
#include <uv.h>

NAMESPACE_START

class TcpServer;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
    friend class TcpServer;

    typedef enum {CONNECTED, FIN_WAIT, CLOSE_WAIT, CLOSED}CONNECTION_STATUS;

    TcpConnection(TcpServer *server, uint64_t id);

    bool connected();

    std::shared_ptr<SockAddr> getLocalAddr();

    std::shared_ptr<SockAddr> getPeerAddr();

    void shutdown();

    void close();

    void send(const std::string &str);

    CONNECTION_STATUS getStatus() const {
        return status_;
    }

    EventLoop* getEventLoop();

private:
    static void shutdownCallback(uv_shutdown_t* handle, int status);

    static void readCallback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

    static void allocCallback(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);

    static void writeComplete(uv_write_t* req, int status);

    static void closeCallback(uv_handle_t* handle);

    typedef enum {BUF_TYPE_STRING} BUF_TYPE;

    struct WriteContext {
        uv_buf_t *buf;
        BUF_TYPE buffType;
        union {
            std::string *str;
        };
        std::shared_ptr<TcpConnection> conn;
    };

private:
    uv_tcp_t *client_;

    TcpServer *server_;

    CONNECTION_STATUS status_;

    std::mutex mutex_;

    Buffer readBuff_;

    uint64_t id_;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

NAMESPACE_END

#endif //MAIN_TCPCONNECTION_H

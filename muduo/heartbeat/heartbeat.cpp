//
// Created by chenchukun on 18/2/27.
//
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <iostream>
#include <set>
#include <functional>
#include <thread>
using namespace std;
using namespace std::placeholders;
using namespace muduo;
using namespace muduo::net;

class HeartbeatServer : noncopyable
{
public:
    HeartbeatServer(EventLoop *loop, InetAddress &addr)
        : loop_(loop)
    {
        server_.reset(new TcpServer(loop_, addr, "HeartbeatServer"));
        server_->setConnectionCallback(bind(&HeartbeatServer::onConnection, this, _1));
        server_->setMessageCallback(bind(&HeartbeatServer::onMessage, this, _1, _2, _3));
    }

    void start() {
        server_->start();
    }

private:
    void onConnection(const TcpConnectionPtr &conn) {
        if (conn->connected()) {
            LOG_INFO << conn->peerAddress().toIpPort() << " online";
            connections_.insert(conn);
        }
        else {
            LOG_INFO << conn->peerAddress().toIpPort() << " offline";
            connections_.erase(conn);
        }
    }

    void onMessage(const TcpConnectionPtr &conn, Buffer *buff, Timestamp timestamp) {
        if (buff->readableBytes() > 0) {
            conn->send(buff);
        }
    }

private:
    typedef set<TcpConnectionPtr> ConnectionList;

    unique_ptr<TcpServer> server_;

    EventLoop *loop_;

    ConnectionList connections_;
};

int main()
{
    Logger::setLogLevel(Logger::INFO);
    EventLoop loop;
    InetAddress address(6180);
    HeartbeatServer server(&loop, address);
    server.start();
    loop.loop();
    return 0;
}

//
// Created by chenchukun on 18/2/27.
//
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <functional>
#include <thread>
#include <queue>
#include <unordered_set>
using namespace std;
using namespace std::placeholders;
using namespace muduo;
using namespace muduo::net;

class HeartbeatServer : noncopyable
{
public:
    HeartbeatServer(EventLoop *loop, InetAddress &addr, int timeout)
        : loop_(loop)
    {
        server_.reset(new TcpServer(loop_, addr, "HeartbeatServer"));
        server_->setConnectionCallback(bind(&HeartbeatServer::onConnection, this, _1));
        server_->setMessageCallback(bind(&HeartbeatServer::onMessage, this, _1, _2, _3));
        loop_->runEvery(10, bind(&HeartbeatServer::printConnection, this));
        for (int i=0; i<timeout; ++i) {
            whell_.push(unordered_set<shared_ptr<Entry>>());
        }
        loop_->runEvery(1, bind(&HeartbeatServer::onTimer, this));
    }

    void start() {
        server_->start();
    }

private:
    void onConnection(const TcpConnectionPtr &conn) {
        if (conn->connected()) {
            LOG_INFO << conn->peerAddress().toIpPort() << " online";
            connections_.insert(conn);
            shared_ptr<Entry> entry(new Entry(conn));
            whell_.back().insert(entry);
            weak_ptr<Entry> weakEntry = entry;
            conn->setContext(weakEntry);
        }
        else {
            LOG_INFO << conn->peerAddress().toIpPort() << " offline";
            connections_.erase(conn);
        }
    }

    void onMessage(const TcpConnectionPtr &conn, Buffer *buff, Timestamp timestamp) {
        assert(!conn->getContext().empty());
        weak_ptr<Entry> weakEntry = boost::any_cast<weak_ptr<Entry>>(conn->getContext());
        shared_ptr<Entry> entry = weakEntry.lock();
        if (entry != NULL) {
            whell_.back().insert(entry);
        }
        LOG_INFO << "readableBytes() = " << buff->readableBytes();
        while (buff->readableBytes() > HEAD_LEN) {
            int32_t len = buff->peekInt32();
            if (buff->readableBytes() >= len + HEAD_LEN) {
                buff->retrieve(HEAD_LEN);
                std::string msg(buff->peek(), len);
                LOG_INFO << "recv " << msg << " from " << conn->peerAddress().toIpPort();
                buff->retrieve(len);
                Buffer retMsg;
                retMsg.appendInt32(len);
                retMsg.append(msg);
                conn->send(&retMsg);
            }
            else {
                break;
            }
        }
    }

    void printConnection() {
        auto it = connections_.begin();
        LOG_INFO << "online: " << connections_.size();
        int i = 1;
        while (it != connections_.end()) {
            LOG_INFO << i++ << ": " << (*it)->peerAddress().toIpPort();
            ++it;
        }
    }

    void onTimer() {
        LOG_INFO << "OnTimer";
        whell_.pop();
        whell_.push(unordered_set<shared_ptr<Entry>>());
    }

private:
    enum {HEAD_LEN = 4};

    typedef set<TcpConnectionPtr> ConnectionList;

    typedef weak_ptr<TcpConnection> WeakTcpConnectionPtr;

    unique_ptr<TcpServer> server_;

    EventLoop *loop_;

    ConnectionList connections_;

private:
    struct Entry
    {
        Entry(const WeakTcpConnectionPtr &weakPtr)
                : weakPtr_(weakPtr)
        {
        }

        ~Entry() {
            TcpConnectionPtr conn = weakPtr_.lock();
            if (conn != NULL) {
                conn->shutdown();
            }
        }

        WeakTcpConnectionPtr weakPtr_;
    };

    queue<unordered_set<shared_ptr<Entry>>> whell_;

};

int main()
{
    Logger::setLogLevel(Logger::INFO);
    EventLoop loop;
    InetAddress address(6180);
    HeartbeatServer server(&loop, address, 10);
    server.start();
    loop.loop();
    return 0;
}

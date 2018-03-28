//
// Created by chenchukun on 18/3/28.
//
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/ThreadLocal.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/InetAddress.h>
#include <iostream>
#include <string>
using namespace muduo::net;
using namespace muduo;
using namespace std;

namespace testEventLoopThread
{
    ThreadLocal<string> id;

    TcpServer *server;

    void initCallback(EventLoop *loop)
    {
        id.value() = "EventLoopThread";
        cout << id.value() << " Init" << endl;
        InetAddress addr(6180);
        server = new TcpServer(loop, addr, "testEventLoopThread");
        server->setMessageCallback([] (const TcpConnectionPtr &conn, Buffer *buff, Timestamp timestamp) {
            conn->send(buff);
            buff->retrieveAll();
        });
        server->start();
    }

    void test()
    {
        id.value() = "MainThread";
        cout << id.value() << " Init" << endl;
        EventLoopThread eventLoopThread(initCallback, "test");
        EventLoop *loop = eventLoopThread.startLoop();
        string s;
        while (cin >> s) {
            cout << s << endl;
        }
        loop->quit();
        delete server;
        cout << id.value() << " Exit" << endl;
    }
}


int main()
{
    testEventLoopThread::test();
    return 0;
}

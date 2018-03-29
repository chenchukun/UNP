//
// Created by chenchukun on 18/3/28.
//
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/EventLoopThreadPool.h>
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
    // 线程本地数据
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
        // EventLoopThread 创建一个运行事件循环的线程,可以传递一个初始化回调函数,
        // 该函数在子线程执行EventLoop.loop()前执行
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


namespace testEventLoopThreadPool
{
    void initCallback(EventLoop *loop)
    {
        cout << CurrentThread::name() << " init" << endl;
    }

    void test()
    {
        EventLoop loop;
        // EventLoopThreadPool创建一个线程池,每个线程执行一个事件循环
        EventLoopThreadPool eventLoopThreadPool(&loop, "EventLoopThread");
        eventLoopThreadPool.setThreadNum(4);
        eventLoopThreadPool.start(initCallback);
        loop.loop();
    }

}

int main()
{
//    testEventLoopThread::test();
    testEventLoopThreadPool::test();
    return 0;
}

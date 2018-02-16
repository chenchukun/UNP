//
// Created by chenchukun on 18/2/15.
//

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <iostream>
using namespace std;
using namespace muduo::net;
using namespace muduo;

void onConnection(TcpConnectionPtr &conn)
{

}

void onMessage(TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
{

}

int main()
{
    EventLoop loop;
    InetAddress listenAddr(6180);
    TcpServer server(&loop, listenAddr, "EchoServer");
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();
    loop.loop();
    return 0;
}

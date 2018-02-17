//
// Created by chenchukun on 18/2/15.
//

#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <iostream>
using namespace std;
using namespace muduo::net;
using namespace muduo;

void onConnection(const TcpConnectionPtr &conn)
{
    cout << conn->peerAddress().toIpPort() << (conn->connected()? " online": " offline") << endl;
}

void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
{
    cout << "onMessage" << endl;
    conn->send(buf);
}

void onWriteComplete(const TcpConnectionPtr &conn)
{
    cout << "onWriteComplete" << endl;
}

int main()
{
    EventLoop loop;
    InetAddress listenAddr(6180);
    TcpServer server(&loop, listenAddr, "EchoServer");
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setWriteCompleteCallback(onWriteComplete);
    server.start();
    loop.loop();
    return 0;
}

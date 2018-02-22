//
// Created by chenchukun on 18/2/20.
//
#include <iostream>
#include <thread>
#include <unistd.h>
#include "../EventLoop.h"
#include "../TcpServer.h"
using namespace std;
using namespace uv_cpp;

int main()
{
    EventLoop eventLoop;

    TcpServer server(&eventLoop);

    server.setConnectionCallback([] (TcpConnectionPtr &conn) {
        if (conn->connected()) {
            cout << "onLine" << endl;
        }
        else {
            cout << "offLine start" << endl;
            conn->shutdown();
            cout << "offLine end" << endl;
        }
    });

    server.setMessageCallback([] (TcpConnectionPtr &conn, Buffer &buffer) {
        cout << "Recv: len = " << buffer.readableBytes() << endl;
        string str = buffer.readAll();
//        conn->send(str);
    });

    server.start(6180);

    eventLoop.run();

    return 0;
}
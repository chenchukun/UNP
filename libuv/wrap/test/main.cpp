//
// Created by chenchukun on 18/2/9.
//

#include "../TcpServer.h"
#include <iostream>
#include <signal.h>
using namespace std;


void connectionCb(TcpConnectionPtr &connectionPtr)
{
    if (connectionPtr->connected()) {
        auto local = connectionPtr->getLocalAddr();
        auto remote = connectionPtr->getRemoteAddr();
        char buff[64];
        uv_ip4_name(local.get(), buff, sizeof(buff));
        cout << "localAddr: " << buff << endl;
        uv_ip4_name(remote.get(), buff, sizeof(buff));
        cout << "remoteAddr: " << buff << endl;
        cout << "connected: " << connectionPtr->connected() << endl;
    }
    else {
        connectionPtr->shutdown();
        cout << "connected: " << connectionPtr->connected() << endl;
    }

}

void messageCallback(TcpConnectionPtr &connectionPtr, Buffer &buffer)
{
    cout << "messageCallback: " << endl;
    cout << "readBytes(): " << buffer.readBytes() << endl;
    string message = buffer.readAll();
//    cout << "message = " << message << endl;
    connectionPtr->send(message);
}

void errorCallback(TcpConnectionPtr &connectionPtr, int errcode)
{
    cout << "errorCallback" << endl;
}

int main()
{
    signal(SIGPIPE, SIG_IGN);

    uv_loop_t *loop = uv_default_loop();

    TcpServer tcpServer(loop);

    tcpServer.setConnectionCallback(connectionCb);
    tcpServer.setMessageCallback(messageCallback);
    tcpServer.setErrorCallback(errorCallback);

    int ret = tcpServer.start(1618);
    if (ret != 0) {
        cerr << "TcpServer::start: " << uv_strerror(ret) << endl;
        exit(ret);
    }

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
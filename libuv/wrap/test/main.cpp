//
// Created by chenchukun on 18/2/9.
//

#include "../TcpServer.h"
#include <iostream>
using namespace std;

void connectionCb(uv_stream_t* server, int status)
{
    cout << "connectionCb" << endl;
}

int main()
{
    uv_loop_t *loop = uv_default_loop();

    TcpServer tcpServer(loop);

    tcpServer.setConnectionCallback(connectionCb);

    int ret = tcpServer.start(1618);
    if (ret != 0) {
        cerr << "TcpServer::start: " << uv_strerror(ret) << endl;
        exit(ret);
    }

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
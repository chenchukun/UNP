//
// Created by chenchukun on 18/1/11.
//
#include <iostream>
#include <netinet/in.h>
#include <uv.h>
#include <string>
using namespace std;

void on_connection(uv_stream_t* server, int status)
{
    if (status < 0) {
        cout << "new connection error: " << uv_strerror(status) << endl;
        return;
    }
}

int main(int argc, char **argv)
{
    uv_loop_t *loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    struct sockaddr_in addr;
    uv_ip4_addr(argv[1], stoi(argv[1]), &addr);
    uv_tcp_bind(&server, (struct sockaddr*)&addr, 0);

    int ret = uv_listen(&server, 1024, on_connection);
    if (ret != 0) {
        cout << "uv_listen error: " << uv_strerror(ret) << endl;
        exit(1);
    }

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);

    return 0;
}

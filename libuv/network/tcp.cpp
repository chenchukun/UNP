//
// Created by chenchukun on 18/1/20.
//
#include <uv.h>
#include <iostream>
using namespace std;

int main()
{
    uv_loop_t *loop = uv_default_loop();

    uv_tcp_t *server = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, server);
    uv_tcp_nodelay(server, 1);

    return 0;
}
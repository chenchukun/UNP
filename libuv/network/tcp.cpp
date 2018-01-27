//
// Created by chenchukun on 18/1/20.
//
#include <uv.h>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

void connection_cb(uv_stream_t *server, int status)
{
    if (status != 0) {
        cerr << "connection_cb: " << uv_strerror(status) << endl;
        return;
    }
    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(server->loop, client);
    // 接收新连接
    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_os_fd_t fd;
        uv_fileno((uv_handle_t*)client, &fd);
        write(fd, "hello\n", 6);
        cout << "client fd = " << fd << endl;

        char buff[64];
        sockaddr_in addr;
        int len = sizeof(addr);
        int ret = uv_tcp_getsockname(client, (sockaddr*)&addr, &len);
        if (ret == 0 && uv_ip4_name(&addr, buff, sizeof(buff)) == 0) {
            cout << "uv_tcp_getsockname = " << buff << endl;
        }
        ret = uv_tcp_getpeername(client, (sockaddr*)&addr, &len);
        if (ret == 0 && uv_ip4_name(&addr, buff, sizeof(buff)) == 0) {
            cout << "uv_tcp_getpeername = " << buff << endl;
        }
    }
    uv_close((uv_handle_t*)client, NULL);
    free(client);
}

int main(int argc, char **argv)
{
    uv_loop_t *loop = uv_default_loop();

    uv_tcp_t *server = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    // 初始化uv_tcp_t,此时并为创建socket套接字
    uv_tcp_init(loop, server);

    uv_tcp_nodelay(server, 1);

    uv_tcp_keepalive(server, 1, 1800);

    /*
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // 使用一个socket fd来创建tcp句柄
    uv_tcp_open(server, fd);
    */

    int send_buff_size = 0;
    // send_buff_size为0则用于获取套接字发送缓冲区大小,大于0则用于设置发送缓冲区大小
    // 相当于SO_SNDBUF
    uv_send_buffer_size((uv_handle_t*)&server, &send_buff_size);
    cout << "send_buff_size = " << send_buff_size << endl;
    send_buff_size = 4096;
    uv_send_buffer_size((uv_handle_t*)&server, &send_buff_size);

    int recv_buffer_size = 0;
    // send_buff_size为0则用于获取套接字接收缓冲区大小,大于0则用于设置接收缓冲区大小
    // 相当于SO_RCVBUF
    uv_recv_buffer_size((uv_handle_t*)&server, &recv_buffer_size);
    cout << "recv_buffer_size = " << recv_buffer_size << endl;
    recv_buffer_size = 4096;
    uv_recv_buffer_size((uv_handle_t*)&server, &recv_buffer_size);


    struct sockaddr_in addr;
    int ret;
    // 初始化sock地址
    ret = uv_ip4_addr(argv[1], stoi(argv[2]), &addr);
    if (ret != 0) {
        cerr << "uv_ip4_addr: " << uv_strerror(ret) << endl;
        exit(-1);
    }
    // 绑定地址,此时若是使用uv_tcp_init创建的handle,会先创建sock fd。
    // 最后一个参数基本没什么用,为0就行
    ret = uv_tcp_bind(server, (sockaddr*)&addr, 0);
    if (ret != 0) {
        cerr << "uv_tcp_bind: " << uv_strerror(ret) << endl;
        exit(-1);
    }

    // 创建监听套接字,当有连接请求到底时调用connection_cb回调
    ret = uv_listen((uv_stream_t*)server, 1024, connection_cb);
    if (ret != 0) {
        cerr << "uv_listen: " << uv_strerror(ret) << endl;
        exit(-1);
    }

    uv_run(loop, UV_RUN_DEFAULT);

    uv_stop(loop);

    return 0;
}
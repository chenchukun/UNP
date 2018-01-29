//
// Created by chenchukun on 18/1/28.
//

#include <uv.h>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <thread>
#include <cstring>
#include <unistd.h>
using namespace std;

void shutdown_cb(uv_shutdown_t *req, int status)
{
    cout << "shutdown_cb: " << endl;
    // handle指向此连接正在运行的流的指针
    uv_read_stop(req->handle);
    uv_close((uv_handle_t*)req->handle, NULL);
    free(req->handle);
    free(req);
}

void write_cb(uv_write_t *req, int  status)
{
    if (status != 0) {
        cerr << "write_cb: " << uv_strerror(status) << endl;
        uv_shutdown_t *req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
        uv_shutdown(req, req->handle, shutdown_cb);
    }
    free(req);
}

void read_cb(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf )
{
//    cout << "read_cb: tid = " << this_thread::get_id() << endl;
    if (nread > 0) {
        cout << "read_cb: " << buf->base << endl;
        uv_write_t *wreq = (uv_write_t*)malloc(sizeof(uv_write_t));
        uv_write(wreq, stream, buf, 1, write_cb);
        if (strncmp(buf->base, "shutdown", 8) == 0) {
            uv_shutdown_t *req = (uv_shutdown_t*)malloc(sizeof(uv_shutdown_t));
            uv_shutdown(req, stream, shutdown_cb);
        }
        return;
    }
    // 读取出错
    if (nread < 0) {
        if (nread != UV_EOF) {
            cerr << "read_cb error: " << uv_strerror(nread) << endl;
        }
        else {
            cout << "client close" << endl;
        }
        uv_read_stop(stream);
        uv_close((uv_handle_t*)stream, NULL);
        free(stream);
    }
    free(buf->base);
}

void alloc_cb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
//    cout << "alloc_cb: tid = " << this_thread::get_id() << endl;
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}


void connection_cb(uv_stream_t *server, int status)
{
//    cout << "connection_cb: tid = " << this_thread::get_id() << endl;
    if (status != 0) {
        cerr << "connection_cb: " << uv_strerror(status) << endl;
        return;
    }
    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(server->loop, client);
    // 接收新连接
    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_read_start((uv_stream_t*)client, alloc_cb, read_cb);
    }
    else {
        uv_close((uv_handle_t*)client, NULL);
        free(client);
    }
}

void timer_cb(uv_timer_t *handle)
{
    cout << "timer_cb" << endl;
    uv_stop(uv_default_loop());
}

int main(int argc, char **argv)
{
//    cout << "main: tid = " << this_thread::get_id() << endl;
    uv_loop_t *loop = uv_default_loop();

    uv_tcp_t *server = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, server);
    uv_tcp_nodelay(server, 1);

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

    uv_timer_t timer;
    uv_timer_init(loop, &timer);
    uv_timer_start(&timer, timer_cb, 30000, 0);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_stop(loop);

    return 0;
}
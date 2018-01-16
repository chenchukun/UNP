//
// Created by chenchukun on 18/1/11.
//
#include <iostream>
#include <netinet/in.h>
#include <uv.h>
#include <string>
#include <signal.h>
using namespace std;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        cerr << "Write error " <<  uv_strerror(status) << endl;
    }
    free_write_req(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        uv_write((uv_write_t*) req, client, &req->buf, 1, echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    }

    free(buf->base);
}

void on_connection(uv_stream_t *server, int status)
{
    if (status < 0) {
        cerr << "new connection error: " << uv_strerror(status) << endl;
        return;
    }
    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(server->loop, client);
    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*)client, NULL);
    }
}

int main(int argc, char **argv)
{
    uv_loop_t *loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    struct sockaddr_in addr;
    uv_ip4_addr(argv[1], stoi(argv[2]), &addr);
    uv_tcp_bind(&server, (struct sockaddr*)&addr, 0);

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

    uv_os_fd_t fd;  // int类型
    // 获取handl对于的文件描述符
    uv_fileno((uv_handle_t*)&server, &fd);
    cout << "uv_fileno = " << fd << endl;

    int ret = uv_listen((uv_stream_t*)&server, 1024, on_connection);
    if (ret != 0) {
        cerr << "uv_listen error: " << uv_strerror(ret) << endl;
        exit(1);
    }

    signal(SIGPIPE, SIG_IGN);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);

    return 0;
}

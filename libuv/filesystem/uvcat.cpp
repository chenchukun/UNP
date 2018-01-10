//
// Created by chenchukun on 18/1/10.
//
#include <iostream>
#include <unistd.h>
#include <uv.h>
using namespace std;

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

static char buffer[1024];

static uv_buf_t iov;

void on_read(uv_fs_t *req);

// 异步写成功回调
void on_write(uv_fs_t *req) {
    // result保存write()的返回值
    if (req->result < 0) {
        cerr << "Write error: " <<  uv_strerror(req->result) << endl;
    }
    else {
        // 继续调用异步读
        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
    }
}

// 异步读成功回调
void on_read(uv_fs_t *req)
{
    // result保存read()的返回值
    if (req->result < 0) {
        cerr << "Read error: " << uv_strerror(req->result) << endl;
    }
    else if (req->result == 0) {
        uv_fs_t close_req;
        // 关闭文件为同步操作
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
    }
    else if (req->result > 0) {
        iov.len = req->result;
        // 读取成功则调用异步写
        uv_fs_write(uv_default_loop(), &write_req, STDOUT_FILENO, &iov, 1, -1, on_write);
    }
}

// 文件打开操作回调
void on_open(uv_fs_t *req)
{
    // result保存文件open()的返回值,大于大于0为文件描述符,小于0为错误码
    if (req->result >= 0) {
        cout << "Open file " << (char*)req->data << " success" << endl;
        // 初始化缓冲区
        iov = uv_buf_init(buffer, sizeof(buffer));
        // 调用异步读
        uv_fs_read(uv_default_loop(), &read_req, req->result,
                   &iov, 1, -1, on_read);
    }
    else {
        cerr << "Open file " << (char*)req->data << " error: " << uv_strerror(req->result) << endl;
    }
}

int main(int argc, char **argv)
{
    uv_loop_t *loop = uv_default_loop();

    /*
     * libuv 支持文件异步操作,内部使用线程池来实现异步读写本地文件
     * uv_fs_open用户异步的打开文件,req指定uv_fs_t类型的handle
     * path 指定文件路径,flags 和 mode与open系统调用类似
     * cb指定文件打开后的回调
     */
    open_req.data = (void*)argv[1];
    uv_fs_open(loop, &open_req, argv[1], O_RDONLY, 0, on_open);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
}
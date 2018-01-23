//
// Created by chenchukun on 18/1/23.
//
#include <uv.h>
#include <iostream>
using namespace std;


void close_cb(uv_handle_t *handle)
{
    cout << "call close_cb" << endl;
}

int main()
{
    uv_loop_t *loop = uv_default_loop();

    int data = 10;
    // uv_handle_t是libuv中所有handle的基本类型,其他所有handle都继承该类型
    uv_handle_t handle;

    // uv_handle_t提供了一组函数,这些函数也适用于其他任何类型的handle

    // 设置和获取用户数据,可以使用该方法在handle上绑定任意类型的数据
    uv_handle_set_data(&handle, &data);
    cout << "data = " << *(int*)uv_handle_get_data(&handle) << endl;

    // 获取handle类型
    uv_handle_type type = uv_handle_get_type(&handle);
    cout << "type = " << type  << endl;
    // 对具体类型有效,uv_handle_t.type为随机值
//    cout << "uv_handle_type_name(type) = " << uv_handle_type_name(type) << endl;

    // 若handle处于活动状态则返回非0,否则返回0:  uv_handle_t.flags & UV__HANDLE_ACTIVE
    cout << "uv_is_active = " << uv_is_active(&handle) << endl;

    // 若handle正在关闭或已关闭,则返回非0,否则返回0:uv_handle_t.flags & (UV_CLOSING |  UV_CLOSED)
    cout << "uv_is_closing = " << uv_is_closing(&handle) << endl;

    // handle引用计数相关操作
    cout << "uv_has_ref = " << uv_has_ref(&handle) << endl;
    uv_ref(&handle);
    cout << "uv_has_ref = " << uv_has_ref(&handle) << endl;
    uv_unref(&handle);
    cout << "uv_has_ref = " << uv_has_ref(&handle) << endl;

    // 获取handle类型的大小
    cout << "uv_handle_size = " << uv_handle_size(handle.type) << endl;

    // 获取handle所在的事件循环
    uv_handle_get_loop(&handle);

    // 获取handle所对应的文件描述符,只有TCP、管道、TTY、UDP、POLL等类型才支持该操作
    uv_os_fd_t fd;
    int ret = uv_fileno(&handle, &fd);
    if (ret == 0) {
        cout << "fd = " << fd << endl;
    }
    else {
        cerr << "uv_fileno: " << uv_strerror(ret) << endl;
    }

    // 用于设置和获取套接字发送缓冲区和接收缓冲区大小。
    // int uv_send_buffer_size(uv_handle_t *  handle，int *  value );
    // int uv_recv_buffer_size(uv_handle_t *  handle，int *  value)

    uv_run(loop, UV_RUN_DEFAULT);
    uv_stop(loop);

    // 请求关闭句柄:对具体类型的handle有效,不能对基本类型uv_handle_t执行该操作
    // uv_close(&handle, close_cb);
    return 0;
}

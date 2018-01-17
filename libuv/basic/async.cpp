//
// Created by chenchukun on 18/1/17.
//
#include <uv.h>
#include <unistd.h>
#include <iostream>
#include <thread>
using namespace std;

void async_cb(uv_async_t *handle)
{
    cout << "call async_cb" << endl;
    cout << "async_cb thread_id = " << this_thread::get_id() << endl;
}

void send_async(uv_async_t *async)
{
    sleep(1);
    cout << "send_async thread_id = " << this_thread::get_id() << endl;
    // 唤醒事件循环并执行回调
    // 若在回调被调用前多次执行send将被忽略,而回调只调用一次
    uv_async_send(async);
    sleep(1);
    uv_async_send(async);
}

int main()
{
    uv_loop_t *loop = uv_default_loop();

    // async可以用于唤醒事件循环,并在事件循环所在线程执行回调
    uv_async_t async;
    // 不需要start
    uv_async_init(loop, &async, async_cb);

    thread t(send_async, &async);
    cout << "main thread_id = " << this_thread::get_id() << endl;
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    return 0;
}


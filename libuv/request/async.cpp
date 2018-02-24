//
// Created by chenchukun on 18/1/17.
//
#include <uv.h>
#include <unistd.h>
#include <iostream>
#include <thread>
using namespace std;

void close_cb(uv_handle_t *handle)
{
    free(handle);
}

void async_cb(uv_async_t *handle)
{
    cout << "async_cb thread_id = " << this_thread::get_id() << endl;
    uv_close(reinterpret_cast<uv_handle_t*>(handle), close_cb);
}

void send_async(uv_loop_t *loop)
{
    for (int i=0; i<2000000; ++i) {
        // async可以用于唤醒事件循环,并在事件循环所在线程执行回调
        uv_async_t *async = static_cast<uv_async_t*>(malloc(sizeof(uv_async_t)));
        // 不需要start
        int ret = uv_async_init(loop, async, async_cb);
        if (ret != 0) {
            cout << "init error: " << ret  << endl;
        }

        // 唤醒事件循环并执行回调
        // 若在回调被调用前多次执行send将被忽略,而回调只调用一次
        ret =  uv_async_send(async);
        cout << i << endl;
        if (ret != 0) {
            cout << "send error: " << ret  << endl;
        }
        usleep(10000);
    }

}

void timer_cb(uv_timer_t *handle)
{
    cout << "timer_cb" << endl;
}

int main()
{
    uv_loop_t *loop = uv_default_loop();

    thread t(send_async, loop);
    t.detach();

    cout << "main thread_id = " << this_thread::get_id() << endl;

    uv_timer_t timer;
    uv_timer_init(loop, &timer);
    uv_timer_start(&timer, timer_cb, 10, 10);

    while (true) {
        int ret = uv_run(loop, UV_RUN_DEFAULT);
        if (ret != 0) {
            cout << "uv_run error: " << ret << endl;
        }
    }

    uv_loop_close(loop);

    return 0;
}


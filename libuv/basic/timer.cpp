//
// Created by chenchukun on 18/1/17.
//

#include <uv.h>
#include <iostream>
#include <unistd.h>
using namespace std;

void timer_cb(uv_timer_t *handle)
{
    int *num = (int*)handle->data;
    cout << "timeout: " << *num << endl;
    (*num)++;
    if (*num == 5) {
        // 获取定时器时间间隔
        cout << "uv_timer_get_repeat = " << uv_timer_get_repeat(handle) << endl;
        // 重设时间间隔,调用后将按照该时间间隔重新执行该定时器,而不管当前计数器已运行多长时间
        uv_timer_set_repeat(handle, 2000);
    }
    if (*num >= 10) {
        cout << "uv_timer_get_repeat = " << uv_timer_get_repeat(handle) << endl;
        cout << "timer stop" << endl;
        // 停止定时器
        uv_timer_stop(handle);
        if (*num == 10) {
            // 重启一个已经启动过的定时器,若该定时器从未启动过将返回UV_EINVAL错误
            int ret = uv_timer_again(handle);
            if (ret == UV_EINVAL) {
                cerr << "uv_timer_again: " << uv_strerror(ret) << endl;
            }
        }
    }
}

void prepare_cb(uv_prepare_t *handle)
{
    cout << "call prepare_cb" << endl;
    uv_prepare_stop(handle);
}

void check_cb(uv_check_t *handle)
{
    cout << "call check_cb" << endl;
    uv_check_stop(handle);
}

void idle_cb(uv_idle_t *handle)
{
    cout << "call idle_cb" << endl;
    usleep(500000);
    uv_idle_stop(handle);
}

int main()
{
    uv_loop_t *loop = uv_default_loop();

    // 定时器handle,用于执行定时任务
    uv_timer_t timer;
    int num = 0;
    timer.data = &num;
    // 初始化handle
    uv_timer_init(loop, &timer);
    // 启动定时器,cb指定回调函数,
    // timeout指定首次超时时间(单位毫秒),若为0则在下一个时间循环迭代中触发回调
    // repeat指定重复触发时间,若为0则不重复触发,非0则在首次触发后间隔repeat毫秒触发
    uv_timer_start(&timer, timer_cb, 10, 1000);

    // prepare handle用于在每个事件循环中执行回调,会在轮询I/O之前执行
    uv_prepare_t prepare;
    uv_prepare_init(loop, &prepare);
    uv_prepare_start(&prepare, prepare_cb);

    // check handle用于在每个事件循环中执行回调,会在轮询I/O之后执行
    uv_check_t check;
    uv_check_init(loop, &check);
    uv_check_start(&check, check_cb);

    // 空闲 handle,用于在每个事件循环中执行一个回调,在uv_prepare_t之前执行。
    // 与uv_prepare_t不同的是,若无活动事件,循环将执行零超时轮询,并执行idle回调而不是阻塞。
    // uv_idle_t不止在没有活动事件(空闲)时回调,而是在每次事件循环中都回调。
    // 若设置了uv_idle_t,在空闲时执行完uv_idle_t回调后,
    // 若有设置uv_prepare_t或uv_check_t也会同时执行这些回调
    uv_idle_t idle;
    uv_idle_init(loop, &idle);
    uv_idle_start(&idle, idle_cb);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);

    return 0;
}

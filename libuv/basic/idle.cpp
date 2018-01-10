//
// Created by chenchukun on 18/1/10.
//

#include <iostream>
#include <cstdlib>
#include <uv.h>
using namespace std;

void idle_handler(uv_idle_t *handle)
{
    handle
    int *num = (int*)handle->data;
    if (*num <= 100) {
        cout << "idle_handler: " << *num  << endl;
        *num += 1;
    }
    else {
        // 停止空闲任务
        uv_idle_stop(handle);
    }
}

int main()
{
    uv_loop_t *loop = uv_default_loop();

    int num = 0;
    // libuv中,使用handle来表示所感兴趣的时间,相当于封装后的文件描述符
    // 创建一个空闲任务handle,并初始化
    uv_idle_t idler;
    // 每个handle都有一个data域,可以用于保存上下文信息
    idler.data = (void*)&num;
    uv_idle_init(loop, &idler);

    // 设置该handle对应的回调
    uv_idle_start(&idler, idle_handler);

    cout << "start idle" << endl;
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    return 0;
}
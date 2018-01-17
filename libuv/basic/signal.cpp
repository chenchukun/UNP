//
// Created by chenchukun on 18/1/17.
//

#include <uv.h>
#include <iostream>
#include <unistd.h>
using namespace std;

void signal_cb(uv_signal_t *handle, int signum)
{
    cout << "call signal_cb, signum = " << signum << endl;
    // 停止捕获SIGINT信号
    uv_signal_stop(handle);
}

int main()
{
    uv_loop_t *loop = uv_default_loop();

    // 捕获SIGINT信号
    uv_signal_t sig;
    uv_signal_init(loop, &sig);
    uv_signal_start(&sig, signal_cb, SIGINT);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_stop(loop);

    return 0;
}

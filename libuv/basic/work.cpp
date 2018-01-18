//
// Created by chenchukun on 18/1/18.
//
#include <uv.h>
#include <iostream>
#include <thread>
using namespace std;

void work_cb(uv_work_t *req)
{
    cout << "call work_cb, thread_id = " << this_thread::get_id() << endl;
}

void after_work_cb(uv_work_t *req, int status)
{
    // 若请求被取消,则status为UV_ECANCELED
    cout << "status = " << status << ", " << uv_strerror(status) << endl;
    cout << "after_work_cb, thread_id = " << this_thread::get_id() << endl;
}

int main()
{
    uv_loop_t *loop = uv_default_loop();

    cout << "main, thread_id = " << this_thread::get_id() << endl;

    // 在线程池中执行一个任何,并在执行成功后在事件循环线程执行一个after_work_cb回调
    uv_work_t work;
    uv_queue_work(loop, &work, work_cb, after_work_cb);

    // 取消请求,work_cb将不会被调用,after_work_cb会被调用,状态为UV_ECANCELED
    uv_cancel((uv_req_t*)&work);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_stop(loop);
    return 0;
}
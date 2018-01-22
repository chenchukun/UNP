//
// Created by chenchukun on 18/1/10.
//
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <uv.h>
using namespace std;

// 创建event-loop

void init1()
{
// 使用malloc分配内存并初始化event-loop,使用uv_loop_init主要是允许自定义内存管理方式
    uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    cout << "init-1" << endl;

    // 获取事件循环的描述符,即epoll的描述符
    cout << "uv_backend_fd = " << uv_backend_fd(loop) << endl;
    // uv_run_mode指定运行模式
    uv_run(loop, UV_RUN_DEFAULT);

    // 关闭event-loop
    uv_loop_close(loop);

    // 回收malloc分配的内存
    free(loop);
}

void init2()
{
    // 直接定义uv_loop_t来得到event-loop对象,这种情况不需要调用uv_loop_init
    uv_loop_t loop;

    cout << "init-2" << endl;
    uv_run(&loop, UV_RUN_DEFAULT);

    // 关闭event-loop
    uv_loop_close(&loop);
}

void init3()
{
    // 若只需要一个event-loop,可以使用uv_default_loop()获取libuv提供的默认event-loop
    // 不需要调用uv_loop_init,也不需要释放内存
    uv_loop_t *loop = uv_default_loop();

    cout << "init-3" << endl;
    uv_run(loop, UV_RUN_DEFAULT);

    // 关闭event-loop
    uv_loop_close(loop);
}

void idle_handler(uv_idle_t *handle)
{
    int *num = (int*)handle->data;
    if (*num <= 10) {
        cout << "idle_handler: " << *num  << endl;
        *num += 1;
    }
    else {
        // 停止空闲任务
        uv_idle_stop(handle);
    }
}

void default_mode()
{
    uv_loop_t *loop = uv_default_loop();
    cout << "default_mode:" << endl;
    uv_idle_t idler;
    uv_idle_init(loop, &idler);
    uv_idle_start(&idler, idle_handler);
    /*
     * UV_RUN_DEFAULT:
     * 一直运行事件循环，直到没有更多的活动和引用句柄或请求。
     * 如果uv_stop() 被调用，仍然有活动的句柄或请求则返回非0
     * 在所有其他情况下返回零。
     */
    int ret = uv_run(loop, UV_RUN_DEFAULT);
    cout << "ret = " << ret << endl;
    uv_loop_close(loop);
}

void once_mode()
{
    uv_loop_t *loop = uv_default_loop();
    cout << "once_mode:" << endl;
    uv_idle_t idler;
    uv_idle_init(loop, &idler);
    uv_idle_start(&idler, idle_handler);
    /*
     * UV_RUN_ONCE:
     * 执行一次I/O循环,如果没有事件,则阻塞,
     * 若没有活动句柄或请求,则返回0,
     * 否则返回非0
     */
    int ret = uv_run(loop, UV_RUN_ONCE);
    cout << "ret = " << ret << endl;
    uv_loop_close(loop);
}

void walk_cb(uv_handle_t *handle, void *arg)
{
    cout << handle->type << endl;
}

void nowait_mode()
{
    uv_loop_t *loop = uv_default_loop();
    cout << "nowait_mode:" << endl;
    uv_idle_t idler;
    uv_idle_init(loop, &idler);
    uv_idle_start(&idler, idle_handler);
    /*
     * UV_RUN_NOWAIT:
     * 执行一次I/O循环,如果没有事件,不阻塞,
     * 若没有活动句柄或请求,则返回0,
     * 否则返回非0
     */
    int ret = uv_run(loop, UV_RUN_NOWAIT);
    // 若事件循环中存在活动句柄或请求,则返回非0,否则返回0
    cout << "uv_loop_alive = " << uv_loop_alive(loop) << endl;
    cout << "ret = " << ret << endl;
    cout << "uv_idle_stop" << endl;
    uv_walk(loop, walk_cb, NULL);
    uv_idle_stop(&idler);
    cout << "uv_loop_alive = " << uv_loop_alive(loop) << endl;
    uv_loop_close(loop);
}

void function()
{
    uv_loop_t *loop = uv_default_loop();
    cout << "function:" << endl;

    int data = 1;
    // data成员可以用于在事件循环上绑定任意类型的数据
    uv_loop_set_data(loop, &data);

    void *pdata = uv_loop_get_data(loop);
    cout << "loop->data = " << *(int*)pdata << endl;

    // 获取uv_loop_t 结构大小
    cout << "uv_loop_size = " << uv_loop_size() << endl;

    // 判断event_loop是否有引用的句柄或请求,
    // 即handle_queue    active_reqs     closing_handles不同时为空
    cout << "uv_loop_alive = " << uv_loop_alive(loop) << endl;

    // 获取事件循环本身的文件描述符
    cout << "uv_backend_fd = " << uv_backend_fd(loop) << endl;

    // 获取轮询超时时间,单位为毫秒(相当于epoll_wait的timeout参数)
    cout << "uv_backend_timeout = " << uv_backend_timeout(loop) << endl;

    // 获取毫秒级的当前时间uv_loop_t的time字段
    // 在初始化uv_loop_t时,会将time设置为从系统启动到现在的时间(clock_gettime),
    // 在之后的每次事件循环中都会更新该时间。
    cout << "uv_now = " << uv_now(loop) << endl;
    // 调用clock_gettime()更新uv_loop_t.time
    uv_update_time(loop);

    // 使用walk_cb回调来遍历事件循环中的所有句柄, arg为传递给回调的参数
    uv_walk(loop, walk_cb, NULL);

    int ret = uv_run(loop, UV_RUN_NOWAIT);

    // 停止事件循环,只是设置uv_loop_t.stop_flag为1,
    // 事件循环会在最近一次超时后退出循环,而不是继续执行循环
    uv_stop(loop);

    // 关闭事件循环,并释放资源,只有在所有handle和请求都完成且关闭才能调用成功,
    // 否则返回UV_EBUSY
    uv_loop_close(loop);
}

int main()
{
    function();
    return 0;
}


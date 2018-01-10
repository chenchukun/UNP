//
// Created by chenchukun on 18/1/10.
//
#include <iostream>
#include <cstdlib>
#include <uv.h>
using namespace std;

// 创建event-loop

void init1()
{
// 使用malloc分配内存并初始化event-loop,使用uv_loop_init主要是允许自定义内存管理方式
    uv_loop_t *loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    cout << "init-1" << endl;
    // ==== UV_RUN_DEFAULT 是啥?
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

int main()
{
    init1();
    return 0;
}


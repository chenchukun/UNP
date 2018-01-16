//
// Created by chenchukun on 18/1/10.
//

#include <iostream>
#include <cstdlib>
#include <uv.h>
using namespace std;

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

int main()
{
    uv_loop_t *loop = uv_default_loop();

    int num = 0;
    // libuv中,使用handle来表示所感兴趣的事件,相当于封装后的文件描述符
    // 使用请求requests来表示单次事件的响应?
    // 创建一个空闲任务handle,并初始化
    uv_idle_t idler;
    // 每个handle都有一个data域,可以用于保存上下文信息
    idler.data = (void*)&num;

    // handle类型
    cout << "handle.type = " << idler.type << endl;
    // handle类型名称,1.19.0后添加的API
    // cout << "uv_handle_type_name = " << uv_handle_type_name(idler.type) << endl;
    uv_idle_init(loop, &idler);

    // 检查handle是否处于活动状态,若是则返回非0,否则返回0
    cout << "uv_is_active = " << uv_is_active((uv_handle_t*)&idler) << endl;

    // 设置该handle对应的回调
    uv_idle_start(&idler, idle_handler);
    // uv_idle_t类型handle在调用uv_idle_start后就处于活动状态
    cout << "uv_is_active = " << uv_is_active((uv_handle_t*)&idler) << endl;

    // handle处于关闭或正在关闭状态,则返回非0,否则返回0
    cout << "uv_is_closing = " << uv_is_closing((uv_handle_t*)&idler) << endl;

    // 返回handle是否被引用,一个handle只有被引用和没有被引用两种状态,uv_ref和uv_unref是幂等的
    cout << "uv_has_ref = " << uv_has_ref((uv_handle_t*)&idler) << endl;
    // 引用handle
    uv_ref((uv_handle_t*)&idler);
    // 取消引用
    uv_unref((uv_handle_t*)&idler);
    cout << "uv_has_ref = " << uv_has_ref((uv_handle_t*)&idler) << endl;

    cout << "start idle" << endl;
    uv_run(loop, UV_RUN_DEFAULT);
    // 关闭handle,可以设置关闭后的回调,具体行为?
    uv_close((uv_handle_t*)&idler, NULL);
    cout << "uv_is_closing = " << uv_is_closing((uv_handle_t*)&idler) << endl;

    uv_loop_close(loop);
    return 0;
}
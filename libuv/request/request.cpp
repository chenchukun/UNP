//
// Created by chenchukun on 18/1/24.
//
#include <uv.h>
#include <iostream>
using namespace std;

int main()
{
    uv_loop_t *loop = uv_default_loop();
    // libuv中的request相当于一次性请求的handle
    uv_req_t req;
    int data = 10;
    uv_req_set_data(&req, &data);
    cout << *(int*)uv_req_get_data(&req) << endl;
    cout << "type = " << uv_req_get_type(&req) << endl;
//    cout << "uv_req_type_name(type) = " << uv_req_type_name(type) << endl;

    // 取消待处理的请求,成功返回0,失败返回错误码
    int ret = uv_cancel(&req);
    if (ret != 0) {
        cerr << "uv_cancel: " << uv_strerror(ret) << endl;
    }

    uv_run(loop, UV_RUN_DEFAULT);
    uv_stop(loop);
    return 0;
}

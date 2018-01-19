//
// Created by chenchukun on 18/1/19.
//
#include <uv.h>
#include <iostream>
using namespace std;

// pthread_mutex_t的别名
uv_mutex_t mutex;
volatile int count = 0;
uv_once_t once = UV_ONCE_INIT;
uv_cond_t cond;

void once_fun()
{
    cout << "call once_fun: " << uv_thread_self() << endl;
}

void thread_fun_1(void *arg)
{
    cout << "thread_fun_1: " << uv_thread_self() << endl;
    uv_once(&once, once_fun);
    for (int i=0; i<10000; ++i) {
        uv_mutex_lock(&mutex);
        ++count;
        uv_mutex_unlock(&mutex);
    }
}

void thread_fun_2(void *arg)
{
    cout << "thread_fun_2: " << uv_thread_self() << endl;
    uv_once(&once, once_fun);
    for (int i=0; i<10000; ++i) {
        uv_mutex_lock(&mutex);
        ++count;
        uv_mutex_unlock(&mutex);
    }
}

void thread_fun_3(void *arg)
{
    uv_mutex_lock(&mutex);
    uv_cond_wait(&cond, &mutex);
    uv_mutex_unlock(&mutex);
    cout << "thread_fun_3 exit" << endl;
    uv_mutex_destroy(&mutex);
    uv_cond_destroy(&cond);
}

int main()
{
    uv_mutex_init(&mutex);
    uv_cond_init(&cond);
    // pthread_thread_t 的别名
    uv_thread_t t1, t2, t3;
    int ret;
    ret = uv_thread_create(&t1, thread_fun_1, NULL);
    if (ret != 0) {
        cerr << "uv_thread_create fail: " << uv_strerror(ret) << endl;
    }
    ret = uv_thread_create(&t2, thread_fun_2, NULL);
    if (ret != 0) {
        cerr << "uv_thread_create fail: " << uv_strerror(ret) << endl;
    }
    ret = uv_thread_create(&t3, thread_fun_3, NULL);
    if (ret != 0) {
        cerr << "uv_thread_create fail: " << uv_strerror(ret) << endl;
    }
    // 判断两个线程对象是否相等,是返回1,否返回0
    cout << uv_thread_equal(&t1, &t2) << endl;

    uv_thread_join(&t1);
    uv_thread_join(&t2);

    uv_cond_signal(&cond);
    cout << "count = " << count << endl;
    return 0;
}

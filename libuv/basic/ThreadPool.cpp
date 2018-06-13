#include <uv.h>
#include <functional>
#include <iostream>

using namespace std;

class ThreadPool
{
public:
    typedef function<void(void)> WorkCallback;
    ThreadPool(uv_loop_t *loop, int num)
        : loop_(loop), threadNum_(num)
    {

    }

    void commit(const WorkCallback &cb) {
        WorkCallback *p = new WorkCallback(cb);
        uv_work_t *req = static_cast<uv_work_t*>(malloc(sizeof(uv_work_t)));
        req->data = static_cast<void*>(p);
        uv_queue_work(loop_, req, ThreadPool::work, ThreadPool::workAfter);
    }

private:
    static void work(uv_work_t *req) {
        WorkCallback *p = static_cast<WorkCallback*>(req->data);
        (*p)();
    }

    static void workAfter(uv_work_t *req, int status) {
        WorkCallback *p = static_cast<WorkCallback*>(req->data);
        delete p;
        free(req);
    }

private:
    uv_loop_t *loop_;

    size_t threadNum_;
};

int main()
{
    uv_loop_t *loop = uv_default_loop();
    ThreadPool threadPool(loop, 4);
    threadPool.commit([] {
        cout << "threadPool" << endl;
    });
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
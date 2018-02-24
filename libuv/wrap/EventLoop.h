//
// Created by chenchukun on 18/2/20.
//

#ifndef MAIN_EVENTLOOP_H
#define MAIN_EVENTLOOP_H

#include <cstdlib>
#include <uv.h>
#include <functional>
#include <thread>
#include <mutex>
#include "common.h"
#include "Timestamp.h"

NAMESPACE_START

typedef std::function<void(void)> AsyncCallback;

typedef std::function<void(void)> TimerCallback;

class EventLoop
{
public:

    EventLoop(const EventLoop&) = delete;

    EventLoop& operator=(const EventLoop&) = delete;

    EventLoop();

    ~EventLoop();

    /**
     * 执行事件循环
     * @param mode
     */
    void run(uv_run_mode mode = UV_RUN_DEFAULT) {
        while (true) {
            uv_run(loop_, mode);
        }
    }

    /**
     * 退出事件循环
     */
    void stop();

    /**
     * 获取libuv的uv_loop_t
     * @return
     */
    uv_loop_t* getLoop() {
        return loop_;
    }

    /**
     * 在事件循环所在线程执行回调
     * @param cb
     */
    void runInLoopThread(AsyncCallback cb);

    /**
     * 获取当前线程的EventLoop
     * @return
     */
    static EventLoop* getCurrThreadEventLoop();

    /**
     * 唤醒事件循环
     */
    void wakeup();

    std::thread::id getThreadId() const {
        return threadId_;
    }

    uv_timer_t* runAt(Timestamp time, TimerCallback cb);

    uv_timer_t* runAfter(uint64_t delay, TimerCallback cb);

    uv_timer_t* runEvery(uint64_t interval, TimerCallback cb);

    void cancel(uv_timer_t *timer);

private:
    static void asyncCallback(uv_async_t *async);

    static void timerCallback(uv_timer_t *timer);

    static void closeCallback(uv_handle_t* handle);

    uv_timer_t* timerRunImpl(Timestamp time, uint64_t interval, TimerCallback cb);

private:
    uv_loop_t *loop_;

    std::mutex mutex_;

    std::thread::id threadId_;
};

NAMESPACE_END

#endif //MAIN_EVENTLOOP_H

//
// Created by chenchukun on 18/2/20.
//
#include <iostream>
#include <thread>
#include <unistd.h>
#include "../EventLoop.h"
using namespace std;
using namespace uv_cpp;

void async()
{
    cout << "async" << endl;
}

void idle()
{
    cout << "idle" << endl;
}


int main()
{
    EventLoop eventLoop;

    cout << Timestamp::now().msec() << endl;

    eventLoop.runAfter(1000, [] () {
        cout << "runAfter(1000): " << Timestamp::now().msec() << endl;
    });

    Timestamp at = Timestamp::now() + Timestamp(5000000);
    eventLoop.runAt(at, [at] () {
        cout << "runAt(" << at.msec() << "): " << Timestamp::now().msec() << endl;
    });

    cout << "main threadId = " << this_thread::get_id() << endl;
    printf("main: loop = %p\n", &eventLoop);

    thread t([&eventLoop] () {
        cout << "child threadId = " << this_thread::get_id() << endl;
        uv_timer_t *timer = eventLoop.runEvery(1000, [] () {
            cout << "runEvery(1000): " << Timestamp::now().msec() << endl;
        });
        sleep(2);
        eventLoop.runInLoopThread( [] () {
            cout << "runInLoopThread threadId = " << this_thread::get_id() << endl;
            EventLoop *loop = EventLoop::getCurrThreadEventLoop();
            printf("runInLoopThread: loop = %p\n", loop);
        });
        sleep(3);
        eventLoop.cancel(timer);
        cout << "eventLoop.cancel()" << endl;
        sleep(1);
        eventLoop.stop();
        cout << "eventLoop.stop()" << endl;
    });
    t.detach();

    eventLoop.run();

    return 0;
}
//
// Created by chenchukun on 18/2/24.
//

#include "../EventLoop.h"
#include <unistd.h>
#include <iostream>
using namespace std;
using namespace uv_cpp;

int main()
{
    EventLoop eventLoop;

    thread t([&eventLoop] {
        for (int i=0; ; ++i) {
            eventLoop.runInLoopThread([i] {
                printf("thread %d\n", i);
            });
        }
    });

    t.detach();

    thread t2([&eventLoop] {
        for (int i=0; ; ++i) {
            eventLoop.runInLoopThread([i] {
                printf("thread %d\n", i);
            });
        }
    });

    t2.detach();

    eventLoop.run();

    return 0;
}
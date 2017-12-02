//
// Created by chenchukun on 17/11/2.
//
#include <iostream>
#include <memory>
using namespace std;

#define defer(fun) std::shared_ptr<void> __DEFER__(NULL, [&](void *p){fun();});

void fun() {
    cout << "fun()" << endl;
}

class Test {
public:
    void fun() {
        cout << "Test::fun()" << endl;
    }
};

int main()
{
    shared_ptr<int> p(new int(19));
    auto f = [] () {cout << "f()" << endl;};
    string str;
    cout << (str.find_first_of("343", 0)==string::npos) << endl;

    Test t;
    defer([&](){t.fun();});

    return 0;
}


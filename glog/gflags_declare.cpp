#include <iostream>
#include <gflags/gflags.h>
using namespace std;

// 在别的文件中可以使用DECLARE来访问gflags变量
DECLARE_string(host);
DECLARE_int32(port);
DECLARE_bool(daemon);

void fun()
{
    cout << "in flags_declare.cpp: host = " << FLAGS_host << endl;
    cout << "in flags_declare.cpp: port = " << FLAGS_port << endl;
    cout << "in flags_declare.cpp: daemon = " << FLAGS_daemon << endl;
}
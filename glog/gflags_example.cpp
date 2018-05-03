#include <gflags/gflags.h>
#include <iostream>

using namespace std;

DEFINE_string(host, "0.0.0.0", "the server host");
DEFINE_int32(port, 6180, "the server port");
DEFINE_bool(daemon, false, "daemon");

void fun();

int main(int argc, char **argv)
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    cout << "host = " << FLAGS_host << endl;
    cout << "port = " << FLAGS_port << endl;
    cout << "daemon = " << FLAGS_daemon << endl;
    fun();
    return 0;
}
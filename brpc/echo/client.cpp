#include <iostream>
#include <brpc/channel.h>
#include "echo.pb.h"
using namespace std;

int main()
{
    brpc::Channel channel;
    brpc::ChannelOptions options;
    options.protocol = "baidu_std";
    options.connection_type = "";
    options.timeout_ms = 100;
    options.max_retry = 3;
    if (channel.Init("127.0.0.1:6180", "", &options) != 0) {
        cerr << "Initialize channel fail" << endl;
        return -1;
    }
    EchoService_Stub stub(&channel);
    int logId = 0;
    string message;
    while (!brpc::IsAskedToQuit()) {
        EchoRequest request;
        EchoResponse response;
        if (!getline(cin, message)) {
            break;
        }
        request.set_message(message);
        brpc::Controller cntl;
        cntl.set_log_id(logId++);
        cntl.http_request().set_content_type("application/json");
        stub.echo(&cntl, &request, &response, NULL);
        if (!cntl.Failed()) {
            cout << "Received response from " << cntl.remote_side()
                << " to " << cntl.local_side() << ": " << response.message()
                << " (attached = " << cntl.response_attachment() << ")"
                << " latency = " << cntl.latency_us() << " us" << endl;
        }
        else {
            cerr << "echo fail: " << cntl.ErrorText() << endl;
        }
    }
    return 0;
}
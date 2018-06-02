#include <iostream>
#include <brpc/server.h>
#include "echo.pb.h"
using namespace std;

class EchoServiceImpl : public EchoService
{
public:
    virtual void echo(google::protobuf::RpcController *cntlBase
            , const EchoRequest *request, EchoResponse *response, google::protobuf::Closure *done)
    {
        brpc::ClosureGuard doneGuard(done);
        brpc::Controller *cntl = static_cast<brpc::Controller*>(cntlBase);
        response->set_message(request->message());
    }
};

int main()
{
    brpc::Server server;
    EchoServiceImpl echoService;
    if (server.AddService(&echoService, brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        cerr << "Add echo service fail" << endl;
        return -1;
    }

    brpc::ServerOptions options;
    options.idle_timeout_sec = 10;
    if (server.Start(6180, &options) != 0) {
        cerr << "Start echo service fail" << endl;
        return -1;
    }

    server.RunUntilAskedToQuit();

    return 0;
}
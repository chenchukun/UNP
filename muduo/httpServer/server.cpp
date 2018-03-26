//
// Created by chenchukun on 18/3/26.
//
#include <muduo/net/EventLoop.h>
#include <muduo/net/http/HttpServer.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/http/HttpRequest.h>
#include <muduo/net/http/HttpResponse.h>
#include <iostream>
#include <string>
#include <map>
using namespace muduo::net;
using namespace muduo;
using namespace std;

int main()
{
    EventLoop eventLoop;
    HttpServer server(&eventLoop, InetAddress(8080), "httpServer");
    server.setHttpCallback([] (const HttpRequest &request, HttpResponse *response) {
        cout << "http version = " << request.getVersion() << endl;
        cout << "requests method = " << request.methodString() << endl;
        map<string, string>  headers = request.headers();
        auto it = headers.begin();
        cout << "headers = " << endl;
        while (it != headers.end()) {
            cout << "\t" << it->first << ": " << it->second << endl;
            ++it;
        }
        cout << "path = " << request.path() << endl;
        cout << "query = " << request.query() << endl;
        cout << "receiveTime = " << request.receiveTime().toString() << endl;
        response->setStatusCode(HttpResponse::k200Ok);
        response->addHeader("key", "value");
        response->setBody("hello world");
    });
    server.setThreadNum(4);
    server.start();
    eventLoop.loop();
    return 0;
}

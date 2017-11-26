//
// Created by chenchukun on 17/11/26.
//
#include <iostream>
#include <memory>
#include <ctime>
#include "codec.h"
#include "search.pb.h"
#include "Dispatcher.h"
using namespace std;
using namespace google::protobuf;

void requestCallback(Dispatcher::MessagePointer messagePointer) {
    cout << "==========requestCallback==========" << endl;
    cout << messagePointer->DebugString() << endl;
}

void defaultCallback(Dispatcher::MessagePointer messagePointer) {
    cout << "==========defaultCallback==========" << endl;
    cout << messagePointer->DebugString() << endl;
}

void responseCallback(Dispatcher::MessagePointer messagePointer) {
    cout << "==========responseCallback==========" << endl;
    cout << messagePointer->DebugString() << endl;
}

int main()
{
    SearchRequest request;
    request.set_keyword("kikuchanj");
    request.set_page(1);
    shared_ptr<string> result(codec::encode(request));
    if (result == NULL) {
        cerr << "编码失败" << endl;
        exit(1);
    }
    cout << *result << endl;
    shared_ptr<Message> message(codec::decode(*result));
    if (message == NULL) {
        cerr << "解码失败" << endl;
        exit(1);
    }
    cout << message->DebugString() << endl;

    SearchResponse response;
    response.set_page(1);
    response.set_pagesize(100);
    SearchResponse::Result *sresult = response.add_results();
    sresult->set_title("kikuchanj");
    sresult->set_content("kikuchanj is no.1");
    sresult->set_timestamp(time(NULL));

    Dispatcher dispatcher;
    dispatcher.registerMessageCallback("SearchRequest", requestCallback);
    dispatcher.onMessage(&request);
    dispatcher.setDefaultCallback(defaultCallback);
    dispatcher.onMessage(&response);
    dispatcher.registerMessageCallback("SearchResponse", responseCallback);
    dispatcher.onMessage(&response);
    return 0;
}
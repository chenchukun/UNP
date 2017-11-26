//
// Created by chenchukun on 17/11/26.
//

#ifndef UNP_DISPATCHER_H
#define UNP_DISPATCHER_H

#include <google/protobuf/message.h>
#include <functional>
#include <map>

class Dispatcher
{
public:
    typedef const google::protobuf::Message* MessagePointer;
    typedef const google::protobuf::Descriptor* DescriptorPointer;
    typedef std::function<void(MessagePointer)> MessageCallback;

    Dispatcher() : defaultCallback(NULL) {

    }

    void onMessage(MessagePointer message) {
        DescriptorPointer descriptor = message->GetDescriptor();
        auto it = callbackMap.find(descriptor);
        if (it == callbackMap.end() && defaultCallback!=NULL) {
            defaultCallback(message);
        }
        else {
            it->second(message);
        }
    }

    void setDefaultCallback(MessageCallback cb) {
        defaultCallback = cb;
    }

    void registerMessageCallback(const std::string &typeName, MessageCallback cb) {
        DescriptorPointer descriptor =
            google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
        callbackMap[descriptor] = cb;
    }

private:
    std::map<DescriptorPointer, MessageCallback> callbackMap;

    MessageCallback defaultCallback;
};

#endif //UNP_DISPATCHER_H

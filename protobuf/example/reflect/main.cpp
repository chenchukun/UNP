//
// Created by chenchukun on 17/11/25.
//
#include <iostream>
#include <string>
#include <ctime>
#include "SearchRequest.pb.h"
#include "SearchResponse.pb.h"
using namespace std;
using namespace google::protobuf;


int converValue(const Message &message, const FieldDescriptor *fieldDescriptor,
    const Reflection *reflection, string &value)
{
    FieldDescriptor::CppType cppType = fieldDescriptor->cpp_type();
    switch (cppType) {
        case FieldDescriptor::CPPTYPE_INT32:
            value = to_string(reflection->GetInt32(message, fieldDescriptor));
            break;
        case FieldDescriptor::CPPTYPE_INT64:
            value = to_string(reflection->GetInt64(message, fieldDescriptor));
            break;
        case FieldDescriptor::CPPTYPE_UINT32:
            value = to_string(reflection->GetUInt32(message, fieldDescriptor));
            break;
        case FieldDescriptor::CPPTYPE_UINT64:
            value = to_string(reflection->GetUInt64(message, fieldDescriptor));
            break;
        case FieldDescriptor::CPPTYPE_DOUBLE:
            value = to_string(reflection->GetDouble(message, fieldDescriptor));
            break;
        case FieldDescriptor::CPPTYPE_FLOAT:
            value = to_string(reflection->GetFloat(message, fieldDescriptor));
            break;
        case FieldDescriptor::CPPTYPE_BOOL:
            value = reflection->GetBool(message, fieldDescriptor) ? "true" : "false";
            break;
        case FieldDescriptor::CPPTYPE_ENUM: {
            int ivalue = reflection->GetEnum(message, fieldDescriptor)->number();
            value = to_string(ivalue);
            break;
        }
        case FieldDescriptor::CPPTYPE_STRING:
            value = "\"" + reflection->GetString(message, fieldDescriptor) + "\"";
            break;
        default:
            value = "";
            return -1;
    }
    return 0;
}

// 将消息序列化为字符串
int serializeMessage(const Message &message, string &serializedString)
{
    const Descriptor *descriptor = message.GetDescriptor();
    const Reflection *reflection = message.GetReflection();
    serializedString += "{";
    for (int i=0; i<descriptor->field_count(); ++i) {
        const FieldDescriptor *fieldDescriptor = descriptor->field(i);
        const string &key = fieldDescriptor->name();
        if (!reflection->HasField(message, fieldDescriptor)) {
            continue;
        }
        serializedString += "\"" + key + "\": ";
        string value;
        // 如何实现?
        if (fieldDescriptor->is_repeated()) {
            continue;
        }
        FieldDescriptor::CppType cppType = fieldDescriptor->cpp_type();
        if (cppType == FieldDescriptor::CPPTYPE_MESSAGE) {
            const Message &messageField = reflection->GetMessage(message, fieldDescriptor);
            int ret = serializeMessage(messageField, serializedString);
            if (ret != 0) {
                serializedString = "";
                return ret;
            }
            serializedString += ", ";
        }
        else {
            int ret = converValue(message, fieldDescriptor, reflection, value);
            if (ret != 0) {
                serializedString = "";
                return -1;
            }
            serializedString += value + ", ";
        }
    }
    serializedString.erase(serializedString.end()-1);
    serializedString.erase(serializedString.end()-1);
    serializedString += "}";
    return 0;
}

void test() {
    SearchResponse searchResponse;
    searchResponse.set_page(1);
    searchResponse.set_pagesize(10000);
    SearchResponse::Result *result = new SearchResponse::Result();
    result->set_title("kikuchanj");
    result->set_content("kikuchanj is no.1");
    result->set_timestamp(time(NULL));
    searchResponse.set_allocated_results(result);
    string serializeString;
    if (serializeMessage(searchResponse, serializeString) != 0) {
        cerr << "序列化失败" << endl;
        exit(1);
    }
    cout << serializeString << endl;
}

int main()
{
    // DescriptorPool可以获取到Descriptor
    // 获取FileDescriptor
    cout << "==========获取FileDescriptor==========" << endl;
    const FileDescriptor *fileDescriptor
        = DescriptorPool::generated_pool()->FindFileByName("SearchRequest.proto");
    cout << fileDescriptor->name() << endl;

    // 获取消息描述
    cout << "==========获取Descriptor==========" << endl;
    const Descriptor *descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName("SearchRequest");
    cout << descriptor->name() << endl;
    // 获取消息中的字段描述
    for (int i=0; i<descriptor->field_count(); ++i) {
        const FieldDescriptor *fieldDescriptor = descriptor->field(i);
        cout << fieldDescriptor->type_name() << "\t" << fieldDescriptor->name() << endl;
    }

    // MessageFactory可以利用Descriptor获取类型的instance
    // 利用Descriptor创建类型
    cout << "==========利用Descriptor创建类型==========" << endl;
    const Message *prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
    Message *message = prototype->New();
    SearchRequest *searchRequest = dynamic_cast<SearchRequest*>(message);
    searchRequest->set_keyword("kikuchanj");
    searchRequest->set_page(1);
    cout << searchRequest->DebugString() << endl;
    delete searchRequest;
    cout << "==========利用反射接口给字段赋值==========" << endl;
    message = prototype->New();
    const Reflection *reflection = message->GetReflection();
    const FieldDescriptor * fieldDescriptor1 = descriptor->FindFieldByName("keyword");
    reflection->SetString(message, fieldDescriptor1, "jjy");
    cout << reflection->GetString(*message, fieldDescriptor1) << endl;
    cout << message->DebugString() << endl;
    delete message;
    cout << "==========利用反射自定义序列化==========" << endl;
    test();
    return 0;
}
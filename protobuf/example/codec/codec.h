//
// Created by chenchukun on 17/11/26.
//

#ifndef UNP_CODEC_H
#define UNP_CODEC_H

#include <string>
#include <netinet/in.h>
#include <google/protobuf/message.h>
#include "../person/person.pb.h"

class codec
{
public:
    enum {HeadLength=sizeof(int32_t)};

    static std::string* encode(const google::protobuf::Message &message) {
        std::string *result = new std::string();
        // 消息类型名
        std::string typeName = message.GetTypeName();
        // 类型名长度,包含\0
        int32_t typeNameSize = static_cast<int32_t>(typeName.size()) + 1;
        // 长度转换为网络字节序
        int32_t nTypeNameSize = htonl(typeNameSize);
        result->append(reinterpret_cast<char*>(&nTypeNameSize), sizeof(int32_t));
        result->append(typeName.c_str(), typeNameSize);
        if (!message.AppendToString(result)) {
            delete result;
            result = NULL;
        }
        return result;
    }

    static google::protobuf::Message *decode(const std::string &result) {
        const char *pResult = result.c_str();
        const int32_t *pTmp = reinterpret_cast<const int32_t*>(pResult);
        int32_t typeNameSize = ntohl(*pTmp);
        pResult += sizeof(int32_t);
        google::protobuf::Message *message = createMessage(pResult);
        if (message != NULL) {
            std::string::size_type len = result.size() - sizeof(int32_t) - typeNameSize;
            message->ParseFromArray(pResult+typeNameSize, len);
        }
        return message;
    }

    static google::protobuf::Message* createMessage(const std::string &typeName) {
        google::protobuf::Message *message = NULL;
        const google::protobuf::Descriptor *descriptor =
                google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
        if (descriptor != NULL) {
            const google::protobuf::Message *prototype =
                    google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
            if (prototype != NULL) {
                message = prototype->New();
            }
        }
        return message;
    }
};

#endif //UNP_CODEC_H

//
// Created by chenchukun on 18/2/12.
//

#ifndef MAIN_BUFFER_H
#define MAIN_BUFFER_H

#include <list>
#include <cstdlib>
#include <map>
#include <string>
#include <assert.h>
#include <uv.h>

class TcpConnection;

class Buffer
{
public:
    friend class TcpConnection;

    enum {DEFAULT_BLOCK_SIZE=2048};

    Buffer()
        : readPos_(0),
          writePos_(0)
    {
        char *first = static_cast<char*>(malloc(sizeof(char)*DEFAULT_BLOCK_SIZE));
        blockList_.push_back(std::make_pair(first, DEFAULT_BLOCK_SIZE));
    }

    ~Buffer();

    size_t readBytes();

    void clear();

    void append(const char *ptr, size_t len);

    void append(const std::string &str) {
        append(str.c_str(), str.size());
    }

    void append(const void *ptr, size_t len) {
        append(static_cast<const char *>(ptr), len);
    }

    void appendInt8(int8_t int8) {
        append(reinterpret_cast<const char*>(&int8), sizeof(int8));
    }

    void appendInt16(int16_t int16) {
        int16_t nint16 = htons(int16);
        append(reinterpret_cast<const char*>(&nint16), sizeof(int16));
    }

    void appendInt32(int32_t int32) {
        int32_t nint32 = htonl(int32);
        append(reinterpret_cast<const char*>(&nint32), sizeof(int32));
    }

    void appendInt64(int64_t int64) {
        int64_t nint64 = htonll(int64);
        append(reinterpret_cast<const char*>(&nint64), sizeof(int64));
    }

    std::string readString(size_t len) {
        assert(readBytes() >= len);
        std::string str;
        str.reserve(len);
        while (len > 0) {
            size_t bytes = firstReadBytes();
            size_t nread = bytes>=len? len: bytes;
            len -= nread;
            str += std::string(blockList_.back().first + readPos_, nread);
            readPos_ += nread;
            if (readPos_ == blockList_.front().second) {
                readPos_ = 0;
                if (blockList_.size() > 1) {
                    auto item = blockList_.front();
                    blockList_.pop_front();
                    free(item.first);
                }
                else {
                    writePos_ = 0;
                }
            }
        }
        return std::move(str);
    }

    int8_t readInt8() {
        std::string str = readString(sizeof(int8_t));
        const int8_t *p = reinterpret_cast<const int8_t*>(str.c_str());
        return ntohl(*p);
    }

    int16_t readInt16() {
        std::string str = readString(sizeof(int16_t));
        const int16_t *p = reinterpret_cast<const int16_t*>(str.c_str());
        return ntohl(*p);
    }

    int32_t readInt32() {
        std::string str = readString(sizeof(int32_t));
        const int32_t *p = reinterpret_cast<const int32_t*>(str.c_str());
        return ntohl(*p);
    }

    int64_t readInt64() {
        std::string str = readString(sizeof(int64_t));
        const int64_t *p = reinterpret_cast<const int64_t*>(str.c_str());
        return ntohl(*p);
    }

    std::string readAll() {
        return readString(readBytes());
    }

private:
    size_t tailAvailBytes() {
        return blockList_.back().second - writePos_;
    }

    size_t firstReadBytes() {
        if (blockList_.size() == 1) {
            return writePos_ - readPos_;
        }
        else {
            return blockList_.back().second - writePos_;
        }
    }

    size_t initUVWriteBuf(uv_write_t *writeReq, uv_buf_t **buf);

    void initUVReadBuf(uv_buf_t *buf);

private:
    std::list<std::pair<char*, size_t>> blockList_;

    size_t readPos_, writePos_;
};

#endif //MAIN_BUFFER_H

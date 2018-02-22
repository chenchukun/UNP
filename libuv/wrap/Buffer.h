//
// Created by chenchukun on 18/2/11.
//

#ifndef MAIN_BUFFER_H
#define MAIN_BUFFER_H

#include <vector>
#include <algorithm>
#include <string>
#include <assert.h>
#include "common.h"
#include <uv.h>

NAMESPACE_START

class TcpConnection;

class Buffer
{
public:
    friend TcpConnection;

    Buffer(size_t cap = 4096)
        : capacity_(cap),
          readPos_(0),
          writePos_(0)
    {
        buffer_.reserve(capacity_);
    }

    size_t capacity() const {
        return capacity_;
    }

    size_t readableBytes() const {
        return writePos_ - readPos_;
    }

    size_t writeableBytes() const {
        return capacity_ - writePos_;
    }

    void clear() {
        readPos_ = writePos_ = 0;
    }

    void compact();

    void resize(size_t size);

    void reset() {
        readPos_ = writePos_ = 0;
    }

    void append(const char *ptr, size_t len) {
        checkSize(len);
        std::copy(ptr, ptr+len, buffer_.begin()+writePos_);
        setWritePosition(writePos_ + len);
    }

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

    int8_t readInt8() {
        assert(readableBytes() >= sizeof(int8_t));
        int8_t *p = reinterpret_cast<int8_t*>(readPos_);
        setReadPosition(readPos_ + sizeof(int8_t));
        return *p;
    }

    int16_t readInt16() {
        assert(readableBytes() >= sizeof(int16_t));
        int16_t *p = reinterpret_cast<int16_t*>(readPos_);
        setReadPosition(readPos_ + sizeof(int16_t));
        return *p;
    }

    int32_t readInt32() {
        assert(readableBytes() >= sizeof(int32_t));
        int32_t *p = reinterpret_cast<int32_t*>(readPos_);
        setReadPosition(readPos_ + sizeof(int32_t));
        return *p;
    }

    int64_t readInt64() {
        assert(readableBytes() >= sizeof(int64_t));
        int64_t *p = reinterpret_cast<int64_t*>(readPos_);
        setReadPosition(readPos_ + sizeof(int64_t));
        return *p;
    }

    std::string readString(size_t len) {
        assert(readableBytes() >= len);
        std::string str(buffer_.data() + readPos_, len);
        setReadPosition(readPos_ + len);
        return std::move(str);
    }

    std::string readAll() {
        std::string str(buffer_.data() + readPos_, readableBytes());
        clear();
        return std::move(str);
    }

    void discard(size_t len) {
        assert(readableBytes() >= len);
        setReadPosition(readPos_ + len);
    }

    const char* peek() {
        return buffer_.data() + readPos_;
    }

private:
    void setReadPosition(size_t pos);

    void setWritePosition(size_t pos);

    void checkSize(size_t len);

    void initUVBuffer(uv_buf_t *buff);

private:
    std::vector<char> buffer_;

    size_t capacity_;

    size_t readPos_;

    size_t writePos_;
};

NAMESPACE_END

#endif //MAIN_Buffer_H

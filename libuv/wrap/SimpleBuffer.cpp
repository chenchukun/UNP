//
// Created by chenchukun on 18/2/11.
//
#include "SimpleBuffer.h"

void SimpleBuffer::setReadPosition(size_t pos)
{
    assert(pos < capacity_ and pos >= 0);
    readPos_ = pos;
    if (readPos_ == writePos_) {
        readPos_ = writePos_ = 0;
    }
}

void SimpleBuffer::setWritePosition(size_t pos)
{
    assert(pos < capacity_ and pos >= 0);
    writePos_ = pos;
    if (readPos_ == writePos_) {
        readPos_ = writePos_ = 0;
    }
}

void SimpleBuffer::compact()
{
    if (readPos_ == writePos_) {
        clear();
    }
    else {
        std::copy(buffer_.begin()+readPos_, buffer_.begin()+writePos_, buffer_.begin());
        writePos_ = readBytes();
        readPos_ = 0;
    }
}

void SimpleBuffer::resize(size_t size)
{
    std::vector<char> newBuf;
    newBuf.reserve(size);
    std::copy(buffer_.begin()+readPos_, buffer_.begin()+writePos_, newBuf.begin());
//        buffer_.swap(newBuf);
    buffer_ = newBuf;
    writePos_ = readBytes();
    readPos_ = 0;
    capacity_ = size;
}

void SimpleBuffer::checkSize(size_t len)
{
    if (writeBytes() < len) {
        if (capacity() - writeBytes() < len) {
            resize(capacity() + len*2);
        }
        else {
            compact();
        }
    }
}

void SimpleBuffer::initUVBuffer(uv_buf_t *buff)
{
    checkSize(128);
    buff->base = buffer_.data() + writePos_;
    buff->len = writeBytes();
}

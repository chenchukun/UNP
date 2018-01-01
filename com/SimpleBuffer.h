//
// Created by chenchukun on 18/1/1.
//

#ifndef MAIN_SIMPLEBUFFER_H
#define MAIN_SIMPLEBUFFER_H

#include <vector>
#include <algorithm>

class SimpleBuffer
{
public:
    typedef std::size_t size_type;
    SimpleBuffer(size_type cap = 4096)
        : capacity_(cap),
          readPos_(0),
          writePos_(0)
    {
        buffer_.reserve(capacity_);
    }

    size_type capacity() const {
        return capacity_;
    }

    size_type readSize() const {
        return writePos_ - readPos_;
    }

    size_type writeSize() const {
        return capacity_ - writePos_;
    }

    void compact() {
        if (readPos_ == writePos_) {
            clear();
        }
        else {
            std::copy(buffer_.begin()+readPos_, buffer_.begin()+writePos_, buffer_.begin());
            writePos_ = readSize();
            readPos_ = 0;
        }
    }

    void clear() {
        readPos_ = writePos_ = 0;
    }

    size_type readPosition() const {
        return readPos_;
    }

    size_type writePosition() {
        return writePos_;
    }

    const char* readHead() const {
        return &buffer_[readPos_];
    }

    char* writeHead() {
        return &buffer_[writePos_];
    }

    void setReadPosition(size_type pos) {
        readPos_ = pos;
        if (readPos_ == writePos_) {
            readPos_ = writePos_ = 0;
        }
    }

    void setWritePosition(size_type pos) {
        writePos_ = pos;
        if (readPos_ == writePos_) {
            readPos_ = writePos_ = 0;
        }
    }

    void resize(size_type size) {
        std::vector<char> newBuf;
        newBuf.reserve(size);
        std::copy(buffer_.begin()+readPos_, buffer_.begin()+writePos_, newBuf.begin());
//        buffer_.swap(newBuf);
        buffer_ = newBuf;
        writePos_ = readSize();
        readPos_ = 0;
    }

private:
    std::vector<char> buffer_;

    size_type capacity_;

    size_type readPos_;

    size_type writePos_;
};

#endif //MAIN_SIMPLEBUFFER_H

//
// Created by chenchukun on 18/2/17.
//

#ifndef MAIN_DATABLOCK_H
#define MAIN_DATABLOCK_H

#include <assert.h>

class DataBlock
{
public:
    DataBlock(size_t size)
        : capacity_(size)
        , readPos_(0)
        , writePos_(0)
    {
        buffer_ = new char[size];
    }

    ~DataBlock() {
        delete [] buffer_;
    }

    size_t capacity() const {
        return capacity_;
    }

    size_t readBytes() const {
        return writePos_ - readPos_;
    }

    size_t writeBytes() const {
        return capacity_ - writePos_;
    }

    const char* readPosition() const {
        return buffer_ + readPos_;
    }

    char *writePosition() {
        return buffer_ + writePos_;
    }

    void setReadPosition(size_t pos) {
        assert(pos <= writePos_);
        readPos_ = pos;
    }

    void setWritePosition(size_t pos) {
        assert(pos >= readPos_ && pos <= capacity_);
        writePos_ = pos;
    }

    void resize(size_t size);

    void compact();

    void clear() {
        readPos_ = writePos_ = 0;
    }

private:
    char *buffer_;

    size_t capacity_;

    size_t readPos_;

    size_t writePos_;
};

#endif //MAIN_DATABLOCK_H

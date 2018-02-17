//
// Created by chenchukun on 18/2/17.
//

#include "DataBlock.h"
#include <algorithm>

void DataBlock::resize(size_t size) {
    if (size <= capacity_) {
        capacity_ = size;
        if (readPos_ >= size) {
            readPos_ = writePos_ = 0;
        }
        else if (writePos_ >= size) {
            writePos_ = size;
        }
    }
    else {
        char *newBuff = new char[size];
        memcpy(newBuff, buffer_+readPos_, readBytes());
        buffer_ = newBuff;
        readPos_ = 0;
        writePos_ = readBytes();
        capacity_ = size;
    }
}

void DataBlock::compact() {
    if (readPos_ == writePos_) {
        clear();
    }
    else {
        std::copy(buffer_+readPos_, buffer_+writePos_, buffer_);
        writePos_ = readBytes();
        readPos_ = 0;
    }
}
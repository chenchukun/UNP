//
// Created by chenchukun on 18/2/12.
//

#include "Buffer.h"
#include <algorithm>
#include <vector>
using namespace std;

Buffer::~Buffer()
{
    clear();
}

size_t Buffer::readBytes()
{
    size_t size = 0;
    for (auto item : blockList_) {
        size += item.second;
    }
    size -= readPos_;
    size -= blockList_.back().second - writePos_;
    return size;
}

void Buffer::clear()
{
    auto it = blockList_.begin();
    while (it != blockList_.end()) {
        auto curr = it;
        ++it;
        free(curr->first);
    }
    blockList_.clear();
}

void Buffer::append(const char *ptr, size_t len)
{
    size_t tail = tailAvailBytes();
    if (tail >= len) {
        copy(ptr, ptr + len, blockList_.back().first + writePos_);
        writePos_ += len;
    }
    else {
        copy(ptr, ptr + tail, blockList_.back().first + writePos_);
        len -= tail;
        size_t newSize = len>DEFAULT_BLOCK_SIZE? len: DEFAULT_BLOCK_SIZE;
        char *newBlock = static_cast<char*>(malloc(newSize));
        copy(ptr + tail, ptr + len + tail, newBlock);
        blockList_.push_back(std::make_pair(newBlock, newSize));
        writePos_ = len;
    }
}

size_t Buffer::initUVWriteBuf(uv_write_t *writeReq, uv_buf_t **buf)
{
    size_t size = blockList_.size();
    *buf = static_cast<uv_buf_t*>(malloc(sizeof(uv_buf_t) * size));
    vector<char*> *backup = new vector<char*>();
    uv_buf_t *p = *buf;
    auto it = blockList_.begin();
    for (size_t i =0; i<size; ++i) {
        if (i == 0) {
            p->base = it->first + readPos_;
            p->len = firstReadBytes();
        }
        else if (i == size-1){
            p->base = it->first;
            p->len = writePos_;
        }
        else {
            p->base = it->first;
            p->len = it->second;
        }
        backup->push_back(it->first);
        ++p;
        ++it;
    }
    writeReq->data = static_cast<void*>(backup);
    blockList_.clear();
    blockList_.push_back(make_pair(static_cast<char*>(malloc(sizeof(char)*DEFAULT_BLOCK_SIZE)), DEFAULT_BLOCK_SIZE));
    readPos_ = writePos_ = 0;
    return size;
}

void Buffer::initUVReadBuf(uv_buf_t *buf)
{
    if (writePos_ == blockList_.back().second) {
        char *newBlock = static_cast<char*>(malloc(DEFAULT_BLOCK_SIZE));
        blockList_.push_back(make_pair(newBlock, DEFAULT_BLOCK_SIZE));
        buf->base = newBlock;
        buf->len = DEFAULT_BLOCK_SIZE;
    }
    else {
        buf->base = blockList_.back().first + writePos_;
        buf->len = blockList_.back().second - writePos_;
    }
}


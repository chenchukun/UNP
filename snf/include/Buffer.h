#ifndef BUFFER_H
#define BUFFER_H
#include "Block.h"
#include <deque>
#include <endian.h>

namespace snf
{


class Buffer
{
	friend std::ostream& operator<<(std::ostream &out, const Buffer &buffer);
public:
	enum {BLOCK_MIN_SIZE=4096};

	Buffer();

	Buffer(Block &block) {
		blocks.push_back(block);
	}

	Buffer(Block &&block) {
		blocks.push_back(block);
	}

	void append(const std::string &str);

	void append(Block &block) {
		blocks.push_back(block);
	}

	void append(Block &&block) {
		blocks.push_back(block);
	}

	void append(const void *base, int n);

	void append(Buffer &buffer);

	void appendInt64(int64_t x) {
		x = htobe64(x);
		append(&x, sizeof(x));
	}

	void appendInt32(int32_t x) {
		x = htobe32(x);
		append(&x, sizeof(x));
	}

	void appendInt16(int16_t x) {
		x = htobe16(x);
		append(&x, sizeof(x));
	}

	void appendInt8(int8_t x) {
		append(&x, sizeof(x));
	}

	size_t size() const;

	Block&& getFirstBlock() {
		assert(!blocks.empty());
		Block block = std::move(blocks.front());
		blocks.pop_front();
		return std::move(block);
	}

	int64_t readInt64();

	int32_t readInt32();

	int16_t readInt16();

	int8_t readInt8();

	size_t getBlockCnt() const {
		return blocks.size();
	}

	void read(void *base, int n);

	void read(Block &block, int n);

	void readAll(Block &block);

	void read(std::string &str, int n);

	void readAll(std::string &str);

	int findCRLF();

	int readFd(int fd);

	int writeFd(int fd);

	void discard(int n);

	void discardAll();

//	uint64_t bufferSize = 0;

private:
	void pushNewBlock(size_t n = BLOCK_MIN_SIZE);

	std::deque<Block> blocks;

};

}

#endif

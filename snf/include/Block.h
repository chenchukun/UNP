#ifndef BLOCK_H
#define BLOCK_H
#include <vector>
#include <memory>
#include <string>
#include <assert.h>
#include <iostream>

namespace snf
{

class Block
{
	// debug
	friend std::ostream& operator<<(std::ostream &out, const Block &block);
public:
	Block(size_t size=128);

	Block(const std::string &str);

	Block(const void *base, size_t n);

	Block(const Block &block);

	Block(Block &&block) noexcept;

	~Block() {}

	void resetIndex() {
		setReadIndex(0);
		setWriteIndex(0);
	}

	const char* getBase() const {
		return &buffer[0];
	}

	char* getBase()  {
		return &buffer[0];
	}

	size_t getReadIndex() const {
		return readIndex;
	}

	void setReadIndex(size_t index) {
		assert(index <= size());
		readIndex = index;
	}

	size_t getWriteIndex() const {
		return writeIndex;
	}

	void setWriteIndex(size_t index) {
		assert(index <= size());
		writeIndex = index;
	}

	int resize(size_t size = 0);

	size_t size() const {
		return buffer.size();
	}

	size_t getUsedSize() const {
		if (readIndex > writeIndex) {
			return 0;
		}
		return writeIndex - readIndex;
	}

	size_t getSpaceSize() const {
		return size() - getUsedSize();
	}

	void append(const std::string str);

	void append(const void *base, size_t n);

private:
	std::vector<char> buffer;

	size_t readIndex;

	size_t writeIndex;
};

}

#endif


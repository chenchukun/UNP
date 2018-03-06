#include "Block.h"

namespace snf
{

Block::Block(size_t size)
	: buffer(size),
	  readIndex(0),
	  writeIndex(0)
{
}

Block::Block(const std::string &str)
	: buffer(str.size()),
	  readIndex(0),
	  writeIndex(str.size())
{
	copy(str.begin(), str.end(), buffer.begin());
}

Block::Block(const void *base, size_t n)
	: buffer(n),
	  readIndex(0),
	  writeIndex(n)
{
	copy(static_cast<const char*>(base), static_cast<const char*>(base)+n, buffer.begin());	
}

Block::Block(const Block &block)
	: buffer(block.buffer),
	  readIndex(block.readIndex),
	  writeIndex(block.writeIndex)
{
}

Block::Block(Block &&block) noexcept
{
	buffer = std::move(block.buffer);
	readIndex = block.readIndex;
	writeIndex = block.writeIndex;
}

int Block::resize(size_t size) 
{
	size_t newSize = size;
	if (size <= buffer.size()) {
		newSize = buffer.size();
	}
	std::vector<char> newBuffer(newSize);
	copy(getBase()+readIndex, getBase()+writeIndex, newBuffer.begin());
	writeIndex = writeIndex - readIndex;
	readIndex = 0;
	buffer = std::move(newBuffer);
}

void Block::append(const std::string str)
{
	if (size() - writeIndex < str.size()) {
		resize(getUsedSize() + str.size());
	}
	copy(str.begin(), str.end(), getBase()+writeIndex);
	setWriteIndex(writeIndex + str.size());
}

void Block::append(const void *base, size_t n)
{
	if (size() - writeIndex < n) {
		resize(getUsedSize() + n);
	}
	copy(static_cast<const char*>(base), static_cast<const char*>(base)+n, buffer.begin()+writeIndex);
	setWriteIndex(writeIndex + n);
}

std::ostream& operator<<(std::ostream &out, const Block &block)
{
	std::string str(block.getBase()+block.getReadIndex(), block.getBase()+block.getWriteIndex());
	out << block.readIndex << "  " << block.writeIndex << "  " << block.size() << "  " << str;
	return out;
}

}



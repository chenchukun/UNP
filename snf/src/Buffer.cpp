#include "Buffer.h"
#include <sys/uio.h>

namespace snf
{

void Buffer::pushNewBlock(size_t n) 
{
	if (n < BLOCK_MIN_SIZE) {
		n = BLOCK_MIN_SIZE;
	}
	Block block(n);
	blocks.push_back(std::move(block));
}

Buffer::Buffer() 
{
	pushNewBlock();
}

void Buffer::append(const std::string &str)
{
	Block &block = blocks.back();
	if (block.getSpaceSize() >= str.size()) {
		block.append(str);
	}
	else {
		pushNewBlock(str.size());
		blocks.back().append(str);
	}
}

void Buffer::append(const void *base, int n)
{
	Block &block = blocks.back();
	if (block.getSpaceSize() >= n) {
		block.append(base, n);
	}
	else {
		pushNewBlock(n);
		blocks.back().append(base, n);
	}
}
void Buffer::append(Buffer &buffer)
{
	for (int i=0; i<buffer.blocks.size(); ++i) {
		blocks.push_back(std::move(buffer.blocks[i]));
	}
	buffer.discardAll();
}

size_t Buffer::size() const
{
	size_t s = 0;
	for (int i=0; i<blocks.size(); ++i) {
		s += blocks[i].getUsedSize();
	}
	return s;
}

int64_t Buffer::readInt64()
{
	assert(size() >= sizeof(int64_t));
	int64_t x;
	read(&x, sizeof(x));
	return be64toh(x);
}

int32_t Buffer::readInt32()
{
	assert(size() >= sizeof(int32_t));
	int32_t x;
	read(&x, sizeof(x));
	return be32toh(x);
}

int16_t Buffer::readInt16()
{
	assert(size() >= sizeof(int16_t));
	int16_t x;
	read(&x, sizeof(x));
	return be16toh(x);
}

int8_t Buffer::readInt8()
{
	assert(size() >= sizeof(int8_t));
	int8_t x;
	read(&x, sizeof(x));
	return x;
}

void Buffer::read(void *base, int n)
{
	assert(size() >= n);
	Block &firstBlock = blocks.front();
	if (firstBlock.getUsedSize() >= n) {
		std::copy(firstBlock.getBase()+firstBlock.getReadIndex(), 
			firstBlock.getBase()+firstBlock.getReadIndex()+n, static_cast<char*>(base));
		firstBlock.setReadIndex(firstBlock.getReadIndex()+n);
		if (firstBlock.getUsedSize() == 0) {
			firstBlock.resetIndex();
			if (blocks.size() > 1) {
				blocks.pop_front();
			}
		}
	}
	else {
		std::copy(firstBlock.getBase()+firstBlock.getReadIndex(), 
			firstBlock.getBase()+firstBlock.getWriteIndex(), static_cast<char*>(base));
		char *newBase = static_cast<char*>(base) + firstBlock.getUsedSize();
		n -= firstBlock.getUsedSize();
		blocks.pop_front();
		this->read(newBase, n);
	}
}

void Buffer::read(Block &block, int n)
{
	assert(size() >= n);
	assert(block.getSpaceSize() >= n);
	char *base = new char[n];
	this->read(base, n);
	block.append(base, n);
	delete base;
}

void Buffer::readAll(Block &block)
{
	assert(block.getSpaceSize() >= this->size());
	std::string all;
	this->readAll(all);
	block.append(all);
}

void Buffer::read(std::string &str, int n)
{
	assert(size() >= n);
	char *base = new char[n];
	this->read(base, n);
	str = std::string(base, n);
	delete base;
}

void Buffer::readAll(std::string &str)
{
	size_t n = this->size();
	char *base = new char[n];
	this->read(base, n);
	str = std::string(base, n);
	delete base;
}

int Buffer::findCRLF()
{
	int index = 0;
	bool prevCR = false;
	for (int i=0; i<blocks.size(); ++i) {
		Block &block = blocks[i];
		for (int j=block.getReadIndex(); j<block.getWriteIndex(); ++j) {
			if (*(block.getBase()+j) == '\r') {
				prevCR = true;
			}
			else if (*(block.getBase()+j) == '\n') {
				if (prevCR == true) {
					return index-1;
				}
				prevCR = false;
			}
			else {
				prevCR = false;
			}
			++index;
		}
	}
	return -1;
}

std::ostream& operator<<(std::ostream &out, const Buffer &buffer)
{
	out << buffer.size() << "  " << buffer.getBlockCnt() << std::endl;
	for (int i=0; i<buffer.blocks.size(); ++i) {
		out << buffer.blocks[i] << std::endl;
	}
	/*
	for (int i=0; i<buffer.blocks.size(); ++i) {
		std::string str(buffer.blocks[i].getBase()+buffer.blocks[i].getReadIndex(), buffer.blocks[i].getUsedSize());
		out << i << std::endl;
		out << str << std::endl;;
	}
	*/
	return out;
}

int Buffer::readFd(int fd)
{
	size_t spaceN = blocks.back().size() - blocks.back().getWriteIndex();
	int n;
	if (spaceN <= BLOCK_MIN_SIZE/3) {
		struct iovec iov[2];
		Block block(BLOCK_MIN_SIZE);
		iov[0].iov_base = blocks.back().getBase()+blocks.back().getWriteIndex();
		iov[0].iov_len = spaceN;
		iov[1].iov_base = block.getBase();
		iov[1].iov_len = BLOCK_MIN_SIZE;
		n = ::readv(fd, iov, 2);
		if (n <= 0) {
			return n;
		}
		if (n <= spaceN) {
			blocks.back().setWriteIndex(blocks.back().getWriteIndex() + n);
		}
		else {
			blocks.back().setWriteIndex(blocks.back().getWriteIndex() + spaceN);
			block.setWriteIndex(n - spaceN);
			blocks.push_back(std::move(block));
		}
	}
	else {
		struct iovec iov[1];
		iov[0].iov_base = blocks.back().getBase()+blocks.back().getWriteIndex();
		iov[0].iov_len = spaceN;
		n = ::readv(fd, iov, 1);
		if (n <= 0) {
			return n;
		}
		blocks.back().setWriteIndex(blocks.back().getWriteIndex() + n);
	}
	return n;
}

int Buffer::writeFd(int fd) 
{
	std::vector<struct iovec> iovs(getBlockCnt());
	int cnt = blocks.size()>2 ? 2 : blocks.size();
	for (int i=0; i<cnt; ++i) {
		iovs[i].iov_base = blocks[i].getBase() + blocks[i].getReadIndex();
		iovs[i].iov_len = blocks[i].getUsedSize();
	}
	int n = ::writev(fd, &iovs[0], getBlockCnt());
	if (n < 0) {
		return n;
	}
	discard(n);
	return n;
}

void Buffer::discard(int n)
{
	assert(size() >= n);
	Block &firstBlock = blocks.front();
	if (firstBlock.getUsedSize() >= n) {
		firstBlock.setReadIndex(firstBlock.getReadIndex()+n);
		if (firstBlock.getUsedSize() == 0) {
			firstBlock.resetIndex();
			if (blocks.size() > 1) {
				blocks.pop_front();
			}
		}
	}
	else {
		n -= firstBlock.getUsedSize();
		blocks.pop_front();
		discard(n);
	}
}

void Buffer::discardAll()
{
	while (blocks.size() > 1) {
		blocks.pop_back();
	}
	blocks.front().resetIndex();
}

}

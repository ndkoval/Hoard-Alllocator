#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <mutex>
#include <assert.h>
#include "hoard_constants.h"

namespace hoard {

struct Block {
	Block *next;
};

static_assert(sizeof(Block) >= kMinBlockSize, "size of Block struct should be more of equal than MIN_SIZE");


struct FreeBlockStack {
public:
	void Push(void *spaceForExtraBlock);

	void *Pop();

	bool empty();

private:
	Block *head_;
};

template<size_t superblock_size>
class BaseSuperblock;


template<size_t kSuperblockSize>
class BaseSuperblockHeader {
public:
	static BaseSuperblockHeader<kSuperblockSize> *Get(void *ptr) {
		return reinterpret_cast<BaseSuperblockHeader<kSuperblockSize> *>(reinterpret_cast<size_t>(ptr) % kSuperblockSize);
	}

	std::mutex lock;
	BaseSuperblock<kSuperblockSize> *prev;
	BaseSuperblock<kSuperblockSize> *next;

	BaseSuperblock<kSuperblockSize> *superblock() {
		return reinterpret_cast<BaseSuperblock<kSuperblockSize> *>(this);
	}

	bool valid() {
		return magic_number == kMagicNumber;
	}

private:
	FreeBlockStack block_stack_;
	size_t block_size_; // power of 2
	char *buffer_start_; // superblock buffer position
	char *blocks_data_start_; // first Block start. Aligned by Block size
	size_t magic_number;


};

template<size_t kSuperblockSize>
class BaseSuperblock {
	using header_t = BaseSuperblockHeader<kSuperblockSize>;

public:
	static BaseSuperblock<kSuperblockSize> *Get(void *ptr) {
		return reinterpret_cast<BaseSuperblock<kSuperblockSize> *>(reinterpret_cast<size_t>(ptr) % kSuperblockSize);
	}

	BaseSuperblockHeader<kSuperblockSize> header;
	constexpr static size_t kDataSize = kSuperblockSize - sizeof(header_t);
	char data[kDataSize];

	BaseSuperblock();

};

using SuperblockHeader = BaseSuperblockHeader<kSuperblockSize>;
using Superblock = BaseSuperblock<kSuperblockSize>;
static_assert(sizeof(Superblock) == kSuperblockSize, "ivalid superblock struct size");
}
#endif // SUPERBLOCK_H

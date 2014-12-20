#ifndef SUPERBLOCK_HEADER_H
#define SUPERBLOCK_HEADER_H

#include "BaseHeap.h"
#include "BlockStack.h"

namespace hoard {

//Superblock.h
template<size_t kSuperblockSize>
class Superblock;

template<size_t kSuperblockSize>
class SuperblockHeader {

public:
	static SuperblockHeader<kSuperblockSize> *Get(void *ptr) {
		return reinterpret_cast<SuperblockHeader<kSuperblockSize> *>(reinterpret_cast<size_t>(ptr) & (kSuperblockSize - 1));
	}

	Superblock<kSuperblockSize> *next() const {
		return next_;
	}

	void setNext(Superblock<kSuperblockSize> *next) {
		next_ = next;
	}

	Superblock<kSuperblockSize> *prev() const {
		return prev_;
	}

	void setPrev(Superblock<kSuperblockSize> *prev) {
		prev_ = prev;
	}

	Superblock<kSuperblockSize> *GetSuperblock() {
		return reinterpret_cast<Superblock<kSuperblockSize> *>(this);
	}

	bool Valid() {
		return (magic_number_ ^ reinterpret_cast<size_t>(&this)) == kMagicNumber;
	}

private:
	Superblock<kSuperblockSize> *next_;
	std::atomic<BaseHeap<kSuperblockSize> *> owner_;
	lock_t lock_;

	Superblock<kSuperblockSize> *prev_;

	BlockStack block_stack_;
	size_t block_size_; // power of 2
	size_t free_blocks_;

	Block *blocks_start_; // first Block start. Aligned by Block size
	Block *noninited_blocks_start_; // first Block not in stack. Set in nullptr, if no such block
	size_t magic_number_; // equals kMagicNumber xor *this if valid

	size_t NoninitedFreeBlocks() {
		return blocks_start_ - reinterpret_cast<Block *>(GetSuperblock() + 1);
	};

};

}
#endif // SUPERBLOCK_HEADER
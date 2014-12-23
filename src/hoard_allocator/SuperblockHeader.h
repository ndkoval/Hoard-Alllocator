#ifndef SUPERBLOCK_HEADER_H
#define SUPERBLOCK_HEADER_H

#include "BaseHeap.h"
#include "BlockStack.h"

namespace hoard {

//Superblock.h
class Superblock;


class SuperblockHeader {

public:
	static SuperblockHeader *Get(void *ptr) {
		return reinterpret_cast<SuperblockHeader *>(reinterpret_cast<size_t>(ptr) & (kSuperblockSize - 1));
	}

	Superblock *next() const {
		return next_;
	}

	void setNext(Superblock *next) {
		next_ = next;
	}

	Superblock *prev() const {
		return prev_;
	}

	void setPrev(Superblock *prev) {
		prev_ = prev;
	}

	BaseHeap *owner() const {
		return owner_;
	}

	Superblock *GetSuperblock() {
		return reinterpret_cast<Superblock *>(this);
	}

	bool IsFree() {
		return noninited_blocks_start_ == nullptr;
	}

	bool Valid() {
		return (magic_number_ ^ reinterpret_cast<size_t>(this)) == kMagicNumber;
	}

private:
	Superblock *next_;
	std::atomic<BaseHeap *> owner_;
	lock_t lock_;

	Superblock *prev_;

	BlockStack block_stack_;
	size_t block_size_; // power of 2
	size_t free_blocks_;

	Block *blocks_start_; // first Block start. Aligned by Block size
	Block *noninited_blocks_start_; // first Block not in stack. Set in nullptr, if no such block
	size_t magic_number_; // equals kMagicNumber xor *this if valid

//	size_t NoninitedFreeBlocks() {
//		return blocks_start_ - reinterpret_cast<Block *>(GetSuperblock() + 1);
//	};

};

}
#endif // SUPERBLOCK_HEADER
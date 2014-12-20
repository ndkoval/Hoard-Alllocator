#ifndef BASE_SUPERBLOCK_HEADER_H
#define BASE_SUPERBLOCK_HEADER_H

#include "BaseSuperblock.h"

namespace hoard {

// BaseSuperblock.h
template<size_t kSuperblockSize>
class BaseSuperblock;

template<size_t kSuperblockSize>
class BaseSuperblockHeader {

public:
	static BaseSuperblockHeader<kSuperblockSize> *Get(void *ptr) {
		return reinterpret_cast<BaseSuperblockHeader<kSuperblockSize> *>(reinterpret_cast<size_t>(ptr) & (kSuperblockSize - 1));
	}

	BaseSuperblock<kSuperblockSize> *next() const {
		return next_;
	}

	void setNext(BaseSuperblock<kSuperblockSize> *next) {
		next_ = next;
	}

	BaseSuperblock<kSuperblockSize> *Superblock() {
		return reinterpret_cast<BaseSuperblock<kSuperblockSize> *>(this);
	}

	bool Valid() {
		return (magic_number_ ^ reinterpret_cast<size_t>(&this)) == kMagicNumber;
	}

private:
	BaseSuperblock<kSuperblockSize> *next_;
	std::atomic<BaseHeap *> owner_;
	lock_t lock_;

	BaseSuperblock<kSuperblockSize> *prev_;

	struct Block {
		Block *next_block_;
	};
	static_assert(sizeof(Block) >= kMinBlockSize, "size of Block struct should be more of equal than MIN_SIZE");

	struct FreeBlockStack {
	private:
		Block *head_;
	public:
		FreeBlockStack() : head_(nullptr) {
		}

		bool empty() {
			return head_ == nullptr;
		}

		void Push(void *spaceForExtraBlock) {
			Block *new_block =  new(spaceForExtraBlock)  Block; // constructs new Block in free space
			new_block->next_block_ = head_;
			head_ = new_block;
		}

		void *Pop() {
			assert(!empty());
			Block *result = head_;
			head_ = head_->next_block_;
			return reinterpret_cast<void *> (result);
		}
	};

	FreeBlockStack block_stack_;
	size_t block_size_; // power of 2
	size_t free_blocks_;

	Block *blocks_start_; // first Block start. Aligned by Block size
	Block *noninited_blocks_start_; // first Block not in stack. Set in nullptr, if no such block
	size_t magic_number_; // equals kMagicNumber xor *this if valid

	size_t NoninitedFreeBlocks() {
		return blocks_start_ - reinterpret_cast<Block *>(Superblock() + 1);
	};

};

using SuperblockHeader = BaseSuperblockHeader<kSuperblockSize>;

}
#endif // BASE_SUPERBLOCK_HEADER
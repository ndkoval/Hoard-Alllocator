#ifndef SUPERBLOCK_HEADER_H
#define SUPERBLOCK_HEADER_H

#include "BaseHeap.h"
#include "BlockStack.h"
#include "Superblock.h"

namespace hoard {

//Superblock.h
class Superblock;


class SuperblockHeader {

public:
	SuperblockHeader() :
                       next_(nullptr),
                       prev_(nullptr),
											 magic_number_(0),
											 block_size_(0),
											 size_(0),
											 blocks_allocated_(0),
											 uninited_blocs_left_(0),
											 blocks_start_(nullptr),
											 noninited_blocks_start_(nullptr)
	{}

	SuperblockHeader(const Superblock &) = delete;
	SuperblockHeader & operator=(const Superblock &) = delete;

	static SuperblockHeader *Get(void *ptr) {
		return reinterpret_cast<SuperblockHeader *>(reinterpret_cast<size_t>(ptr) & (kSuperblockSize - 1));
	}

	void * Alloc() {
		assert(blocks_allocated_ < block_size_);
		void * result;
		if(uninited_blocs_left_) {
			result = noninited_blocks_start_;
			noninited_blocks_start_ += block_size_;
			--uninited_blocs_left_;
		} else {
			result = block_stack_.Pop();
		}
		++blocks_allocated_;
		return result;
	}

	void Free(void * ptr) {
		assert(blocks_allocated_ > 0);
		char * byte_ptr = reinterpret_cast<char *>(ptr);
		assert(blocks_start_ <= byte_ptr);
		assert(byte_ptr < noninited_blocks_start_);
		block_stack_.Push(reinterpret_cast<Block *>(ptr));
		--blocks_allocated_;
	}

	void Init(size_t block_size) {
		assert(blocks_allocated_ == 0 && "only free Superblock can be inited");
		block_size_ = block_size;
		size_ = (kSuperblockSize - sizeof(SuperblockHeader)) / block_size;
		blocks_start_ = reinterpret_cast<char *> (this) + RoundUp(sizeof(SuperblockHeader), block_size);
		noninited_blocks_start_ = blocks_start_;
		assert(size_ == (reinterpret_cast<char *>(this) + kSuperblockSize - blocks_start_) / block_size); // todo remove before release
		blocks_allocated_ = 0;
		uninited_blocs_left_ = size_;
		magic_number_ = GetSuperblockMagic();
	}

	Superblock *next() const {
		return next_;
	}

	void set_next(Superblock *next) {
		next_ = next;
	}

	Superblock *prev() const {
		return prev_;
	}

	void set_prev(Superblock *prev) {
		prev_ = prev;
	}

	BaseHeap * owner() const {
		return owner_;
	}

	Superblock * superblock() const {
		return reinterpret_cast<Superblock *>(const_cast<SuperblockHeader *>(this));
	}

	size_t size() {
		return size_;
	}
	size_t blocks_allocated() {
		return blocks_allocated_;
	}

	size_t block_size() {
		return block_size_;
	}

	bool empty() {
		return blocks_allocated_ == 0;
	}

	bool valid() {
		return magic_number_ == GetSuperblockMagic();
	}

private:
	lock_t lock_;
	std::atomic<BaseHeap *> owner_;
	Superblock *next_;
	Superblock *prev_;

	BlockStack block_stack_;
	size_t magic_number_; // equals kMagicNumber xor *this if valid
	size_t block_size_; // power of 2
	size_t size_;
	size_t blocks_allocated_;
	size_t uninited_blocs_left_;

	char *blocks_start_; // first Block start. Aligned by Block size
	char *noninited_blocks_start_; // first Block not in stack. Set in nullptr, if no such block

	size_t GetSuperblockMagic() {
		return reinterpret_cast<size_t>(this) ^ kMagicNumber;
	}

//	size_t GetBlockMagic(char * block) {
//		return std::hash<char *>()(block) ^ GetSuperblockMagic();
//	}

};

}
#endif // SUPERBLOCK_HEADER
#ifndef SUPERBLOCK_HEADER_H
#define SUPERBLOCK_HEADER_H

#include <utils.h>
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
											 one_block_size_(0),
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
    assert(valid());
		assert(blocks_allocated_ < size_);
		void * result;
//		if(uninited_blocs_left_ > 0) { // another allocation strategy
    if(block_stack_.IsEmpty()) {
			result = noninited_blocks_start_;
			noninited_blocks_start_ += one_block_size_;
			--uninited_blocs_left_;
		} else {
			result = block_stack_.Pop();

      #ifndef NDEBUG
      Block * block = reinterpret_cast<Block*>(result);
      Block *const end_block = block + (one_block_size_ / sizeof(Block));
      ++block;
      for (; block < end_block; ++block) {
        assert(block->IsMagic() && "Freed block was corrupted");
      }
      #endif
		}
    CheckBlockValidness(result);
    ++blocks_allocated_;
		return result;
	}

	void Free(void * ptr) {
    assert(valid());
		assert(blocks_allocated_ > 0);
    CheckBlockValidness(ptr);
    Block * block = reinterpret_cast<Block *>(ptr);
    block_stack_.Push(block);
    --blocks_allocated_;

    #ifndef NDEBUG
    Block *const end_block = block + (one_block_size_ / sizeof(Block));
    ++block;
    for (; block < end_block; ++block) {
      block->MakeMagic();
    }
    #endif

	}


	void Init(size_t block_size) {
		assert(blocks_allocated_ == 0 && "only free Superblock can be inited");
    assert(IsPowerOf2(block_size));
    assert(block_size < kSuperblockSize && "Block must bee smaller than Superblock");
    assert(block_size >= kMinBlockSize && "Too small one_block_size");
		one_block_size_ = block_size;
		size_ = (kSuperblockSize - sizeof(SuperblockHeader)) / one_block_size_;
		blocks_start_ = reinterpret_cast<char *> (this) + RoundUp(sizeof(SuperblockHeader), one_block_size_);
    assert(reinterpret_cast<size_t>(blocks_start_) % one_block_size_ == 0 && "Invalid block_start allignment");
		noninited_blocks_start_ = blocks_start_;
		assert(size_ == (reinterpret_cast<char *>(this) + kSuperblockSize - blocks_start_) / block_size); // todo remove before release
		blocks_allocated_ = 0;
		uninited_blocs_left_ = size_;
		magic_number_ = GetSuperblockMagic();
    block_stack_.Reset();
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

	void set_owner(BaseHeap *owner) {
		owner_ = owner;
	}

	Superblock *GetSuperblock() {
		return reinterpret_cast<Superblock *>(this);
	}

	size_t size() const {
		return size_;
	}
	size_t blocks_allocated() const {
		return blocks_allocated_;
	}

	size_t one_block_size() const {
		return one_block_size_;
	}

	bool empty() const {
		return blocks_allocated_ == 0;
	}

	bool valid() const {
		return magic_number_ == GetSuperblockMagic();
	}

  std::unique_lock<hoard::lock_t> GetOwnerLock() {
    BaseHeap *block_owner = owner();
    std::unique_lock<hoard::lock_t> owner_lock(block_owner->lock, std::defer_lock_t());

    while (block_owner != owner()) {
      block_owner = owner();
      owner_lock = std::unique_lock<hoard::lock_t>(block_owner->lock, std::defer_lock_t());
    }
    return std::move(owner_lock);
  }

protected:
  void CheckBlockValidness(void *ptr) {
    char *byte_ptr = reinterpret_cast<char *>(ptr);
    assert(blocks_start_ <= byte_ptr && "Freed block not in valid range");
    assert(byte_ptr < noninited_blocks_start_ && "Freed block not in valid range");
    assert(reinterpret_cast<size_t>(byte_ptr) % one_block_size_ == 0 && "Invalid block allignment");
  };

private:
	lock_t lock_;

  //this fields must be changed only with owner lock taken
	std::atomic<BaseHeap *> owner_;
	Superblock *next_;
	Superblock *prev_;

  //this fields must be changed only with header lock taken
	BlockStack block_stack_;
	size_t magic_number_; // equals kMagicNumber xor *this if valid
	size_t one_block_size_; // power of 2
	size_t size_;
	size_t blocks_allocated_;
	size_t uninited_blocs_left_;

	char *blocks_start_; // first Block start. Aligned by Block size
	char *noninited_blocks_start_; // first Block not in stack. Set in nullptr, if no such block

	size_t GetSuperblockMagic() const {
		return reinterpret_cast<size_t>(this) ^ kMagicNumber;
	}

//	size_t GetBlockMagic(char * block) {
//		return std::hash<char *>()(block) ^ GetSuperblockMagic();
//	}

};

}
#endif // SUPERBLOCK_HEADER
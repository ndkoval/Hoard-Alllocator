#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <mutex>
#include <atomic>
#include <assert.h>
#include "hoard_constants.h"
#include "BaseHeap.h"

namespace hoard {

template<size_t superblock_size>
class BaseSuperblock;


template<size_t kSuperblockSize>
class BaseSuperblockHeader {


public:
  static BaseSuperblockHeader<kSuperblockSize> *Get(void *ptr) {
    return reinterpret_cast<BaseSuperblockHeader<kSuperblockSize> *>(reinterpret_cast<size_t>(ptr) % kSuperblockSize);
  }

  std::atomic<BaseHeap *> owner;
  hoard::lock_t lock;
  BaseSuperblock<kSuperblockSize> *prev;
  BaseSuperblock<kSuperblockSize> *next;

  BaseSuperblock<kSuperblockSize> *superblock() {
    return reinterpret_cast<BaseSuperblock<kSuperblockSize> *>(this);
  }

  bool valid();


private:
  struct Block {
    Block *next;
  };
  static_assert(sizeof(Block) >= kMinBlockSize, "size of Block struct should be more of equal than MIN_SIZE");

  struct FreeBlockStack {
  private:
    Block *head_;
  public:
    FreeBlockStack();

    bool empty();

    void Push(void *spaceForExtraBlock);

    void *Pop();
  };

  FreeBlockStack block_stack_;
  size_t block_size_; // power of 2
  size_t free_blocks_;

  Block *blocks_start_; // first Block start. Aligned by Block size
  Block *noninited_blocks_start_; // first Block not in stack. Set in nullptr, if no such block
  size_t magic_number_; // equals kMagicNumber xor *this if valid
  size_t noninited_free_blocks() {
    return blocks_start_ - reinterpret_cast<Block *>(superblock() + 1);
  };


};


template<size_t kSuperblockSize>
class BaseSuperblock {
  using header_t = BaseSuperblockHeader<kSuperblockSize>;

public:
  BaseSuperblock() {
  };

  static BaseSuperblock<kSuperblockSize> *Get(void *ptr) {
    return reinterpret_cast<BaseSuperblock<kSuperblockSize> *>(reinterpret_cast<size_t>(ptr) % kSuperblockSize);
  }

private:
  BaseSuperblockHeader<kSuperblockSize> header_;
  constexpr static size_t kDataSize = kSuperblockSize - sizeof(header_t);
  char data[kDataSize];


public:
  BaseSuperblockHeader<kSuperblockSize> &header() {
    return header_;
  };


};

//
//
// methods
//
//

template<size_t kSuperblockSize>
BaseSuperblockHeader<kSuperblockSize>::FreeBlockStack::FreeBlockStack() : head_(nullptr) {
}

template<size_t kSuperblockSize>
bool BaseSuperblockHeader<kSuperblockSize>::FreeBlockStack::empty() {
  return head_ == nullptr;
}

template<size_t kSuperblockSize>
void *BaseSuperblockHeader<kSuperblockSize>::FreeBlockStack::Pop() {
  assert(!empty());
  Block *result = head_;
  head_ = head_->next;
  return reinterpret_cast<void *> (result);
}

template<size_t kSuperblockSize>
void BaseSuperblockHeader<kSuperblockSize>::FreeBlockStack::Push(void *spaceForExtraBlock) {
  head_ = new(spaceForExtraBlock)  Block; // constructs new Block in free space
}

template<size_t kSuperblockSize>
bool BaseSuperblockHeader<kSuperblockSize>::valid() {
  return (magic_number_ ^ reinterpret_cast<size_t>(*this)) == kMagicNumber;
}


using SuperblockHeader = BaseSuperblockHeader<kSuperblockSize>;
using Superblock = BaseSuperblock<kSuperblockSize>;
static_assert(sizeof(Superblock) == kSuperblockSize, "ivalid superblock struct size");
}
#endif // SUPERBLOCK_H

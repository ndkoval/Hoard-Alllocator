#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <mutex>
#include <assert.h>
#include "hoard_constants.h"

namespace hoard {

struct Block {
  Block *next;
};

static_assert(sizeof(Block) >= MIN_BLOCK_SIZE, "size of Block struct should be more of equal than MIN_SIZE");


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
class SuperblockHeader {
public:
  std::mutex lock;

  BaseSuperblock<kSuperblockSize> *prev;
  BaseSuperblock<kSuperblockSize> *next;
private:
  size_t block_size_; // power of 2
  char *buffer_start_; // superblock buffer position
  char *blocks_data_start_; // first Block start. Aligned by Block size


};

template<size_t kSuperblockSize>
class BaseSuperblock {
  using header_t = SuperblockHeader<kSuperblockSize>;

public:
  SuperblockHeader<kSuperblockSize> header;
  constexpr static unsigned kDataSize = kSuperblockSize - sizeof(header_t);
  char data[kDataSize];

  BaseSuperblock();

};

using superblock = BaseSuperblock<SUPERBLOCK_SIZE>;
static_assert(sizeof(superblock) == SUPERBLOCK_SIZE, "ivalid superblock struct size");
}
#endif // SUPERBLOCK_H

#ifndef HOARDALLOCATOR_H
#define HOARDALLOCATOR_H

#include<cstdlib>
#include<unistd.h>

namespace hoard {

namespace {

constexpr size_t BLOCK_FACTOR = 2; // power of 2  and > 1
constexpr size_t MIN_BLOCK_SIZE = 8;  //power of BLOCK_FACTOR
static_assert(MIN_BLOCK_SIZE >= sizeof(void *), "too small min Block size");
constexpr size_t EMPTYNESS_FACTOR = 4; // power of 2
constexpr size_t SUPERBLOCK_SIZE = 4 * 1024 * 8; // power of 2
constexpr int MAGIC_NUMBER = 0xdeadbeef;
constexpr size_t DEFAULT_ALIGNMENT = 8;

}

extern const size_t PAGE_SIZE;
extern const size_t PAGES_IN_SUPERBLOCK; // power of 2
extern const size_t NUMBER_OF_CPU;
extern const size_t NUMBER_OF_HEAPS;

}


#endif // HOARDALLOCATOR_H

#ifndef HOARD_HOARD_COMMON_HOARD_CONSTANTS_H
#define HOARD_HOARD_COMMON_HOARD_CONSTANTS_H

#include<cstdlib>
#include<unistd.h>

#ifndef __THROW
#define __THROW
#endif

namespace hoard {

namespace {

constexpr size_t BLOCK_FACTOR = 2; // power of 2  and > 1
constexpr size_t MIN_BLOCK_SIZE = 8;  //power of BLOCK_FACTOR
static_assert(MIN_BLOCK_SIZE >= sizeof(void *), "too small min Block size");
constexpr size_t EMPTYNESS_FACTOR = 4; // power of 2
constexpr size_t SUPERBLOCK_SIZE = 4 * 1024 * 8; // power of 2
constexpr int MAGIC_NUMBER = 0xdeadbeef;
constexpr size_t DEFAULT_ALIGNMENT = 8;

//must be initialized in .cpp that using them
const size_t PAGE_SIZE = sysconf(_SC_PAGESIZE);
const size_t PAGES_IN_SUPERBLOCK = SUPERBLOCK_SIZE / PAGE_SIZE; // power of 2
const size_t NUMBER_OF_CPU = sysconf(_SC_NPROCESSORS_ONLN);
const size_t NUMBER_OF_HEAPS = NUMBER_OF_CPU * 2;
}


}


#endif // HOARDALLOCATOR_H

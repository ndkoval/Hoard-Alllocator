#ifndef HOARD_HOARD_COMMON_HOARD_CONSTANTS_H
#define HOARD_HOARD_COMMON_HOARD_CONSTANTS_H

#include<stdlib.h>
#include<unistd.h>
#include <mutex>

#ifndef __THROW
#define __THROW
#endif

namespace hoard {

using lock_t = std::mutex;
using lock_guard = std::lock_guard<lock_t>;


namespace {

constexpr size_t kPageSize = 4 * 1024; //need to be constexpr. Checking at runtime, that it equal real page size
constexpr size_t kPagesInSuperblock = 4; // power of 2
constexpr size_t kSuperblockSize = kPageSize * kPagesInSuperblock; // power of 2
constexpr size_t kMinBlockSize = sizeof(void *);  //power of 2
constexpr size_t kMaxBlockSize = kSuperblockSize / 4; // power of 2, less then kSuperblockSize
static_assert(kMinBlockSize >= sizeof(void *), "too small min Block size");
static_assert(kMaxBlockSize < kSuperblockSize, "too big max Block size");
static_assert(kMaxBlockSize > kMinBlockSize, "illegal block sizes");
constexpr size_t kEmptynessFactor = 8; // power of 2. WARNING: after change this parameter some code changes in LocalHeap bins functions will be needed
constexpr size_t kMagicNumber = 0xdeadbeefdeadbeef;
constexpr size_t kDefaultAlignment = kMinBlockSize; // can't be changed
constexpr size_t kDefaultMapNewSuperblocksCount = 8;
constexpr size_t kMaxFreeSuperblocks = 32;
static_assert(kMaxFreeSuperblocks > kDefaultMapNewSuperblocksCount, "illegal constants");
constexpr size_t kSuperblocksInLocalHeapLowBound = 1;
constexpr size_t kHeapsPerCPU = 2;

}

}
#endif // HOARDALLOCATOR_H

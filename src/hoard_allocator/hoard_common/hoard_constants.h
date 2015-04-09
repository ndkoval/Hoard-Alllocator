#ifndef HOARD_HOARD_COMMON_HOARD_CONSTANTS_H
#define HOARD_HOARD_COMMON_HOARD_CONSTANTS_H

#include<stdlib.h>
#include<unistd.h>
#include <mutex>

#ifndef __THROW
#define __THROW
#endif

namespace hoard {
  namespace details {
    constexpr inline bool IsPowerOf2(size_t n) {
      return ((n != 0) && !(n & (n - 1)));
    }
  }

using lock_t = std::mutex;
using lock_guard = std::lock_guard<lock_t>;

#define HOARD_USE_SUPERBLOCK_MANAGER

namespace {

constexpr size_t kPageSize = 4 * 1024; //need to be constexpr. Checking at runtime, that it equal real page size

//CHANGEABLE
constexpr size_t kSuperblockSize = kPageSize * 4;  //power of 2
constexpr size_t kDefaultMapNewSuperblocksCount = 16;
constexpr size_t kMaxFreeSuperblocks = 32;
constexpr size_t kSuperblocksInLocalHeapLowBound = 4;
constexpr size_t kHeapsPerCPU = 2; //2 is optimal
constexpr size_t kMinBlockSize = sizeof(void *); //minimum size of pointer, power of 2
constexpr size_t kMaxBlockSize = kSuperblockSize / 4;  // power of 2, less then kSuperblockSize

constexpr size_t kEmptynessFactor = 8; // power of 2. WARNING, HARDCODED: after change this parameter some code changes in LocalHeap bins functions will be needed

//FIXED
constexpr size_t kMagicNumber = 0xdeadbeefdeadbeef;
constexpr size_t kDefaultAlignment = kMinBlockSize; // FIXED

static_assert(details::IsPowerOf2(kSuperblockSize), "invalid constant");
static_assert(kMaxFreeSuperblocks > kDefaultMapNewSuperblocksCount, "invalid constants");
static_assert(kMinBlockSize >= sizeof(void *), "too small min Block size");
static_assert(details::IsPowerOf2(kMinBlockSize), "invalid constant");
static_assert(details::IsPowerOf2(kMaxBlockSize), "invalid constant");
static_assert(kMaxBlockSize < kSuperblockSize, "too big max Block size");
static_assert(kMaxBlockSize > kMinBlockSize, "illegal block sizes");
}

}
#endif // HOARDALLOCATOR_H

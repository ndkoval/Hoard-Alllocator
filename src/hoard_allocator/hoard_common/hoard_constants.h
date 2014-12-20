#ifndef HOARD_HOARD_COMMON_HOARD_CONSTANTS_H
#define HOARD_HOARD_COMMON_HOARD_CONSTANTS_H

#include<cstdlib>
#include<unistd.h>
#include <mutex>

#ifndef __THROW
#define __THROW
#endif

namespace hoard {

using lock_t = std::mutex;
using lock_guard = std::lock_guard<lock_t>;


namespace {

constexpr size_t kPageSize = 4 * 1024; //need to be constexpr for performance reason
constexpr size_t kSuperblockSize = 4 * 1024 * 8; // power of 2
constexpr size_t kPagesInSuperblock = kSuperblockSize / kPageSize; // power of 2
constexpr size_t kBlockFactor = 2; // power of 2  and > 1
constexpr size_t kMinBlockSize = sizeof(void *);  //power of kBlockFactor
constexpr size_t kMaxBlockSize = kSuperblockSize / 4;
static_assert(kMinBlockSize >= sizeof(void *), "too small min Block size");
constexpr size_t kEmptynessFactor = 8; // power of 2
constexpr size_t kMagicNumber = 0xdeadbeef;
constexpr size_t kDefaultAlignment = sizeof(void *);
constexpr size_t kDefaultMapNewSuperblocksCount = 16;
constexpr size_t kMaxFreeSuperblocks = 64;

}

}
#endif // HOARDALLOCATOR_H

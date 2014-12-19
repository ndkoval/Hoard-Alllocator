#ifndef HOARD_HOARD_COMMON_HOARD_CONSTANTS_H
#define HOARD_HOARD_COMMON_HOARD_CONSTANTS_H

#include<cstdlib>
#include<unistd.h>

#ifndef __THROW
#define __THROW
#endif

namespace hoard {

namespace {

constexpr size_t kPageSize = 4 * 1024; //need to be constexpr for performance reason
constexpr size_t kSuperblockSize = 4 * 1024 * 8; // power of 2
constexpr size_t kPagesInSuperblock = kSuperblockSize / kPageSize; // power of 2
constexpr size_t kBlockFactor = 2; // power of 2  and > 1
constexpr size_t kMinBlockSize = 8;  //power of kBlockFactor
constexpr size_t kMaxBlockSize = kSuperblockSize / 4;
static_assert(kMinBlockSize >= sizeof(void *), "too small min Block size");
constexpr size_t kEmptynessFactor = 4; // power of 2
constexpr size_t kMagicNumber = 0xdeadbeef;
constexpr size_t kDefaultAlignment = 8;


}

}

#endif // HOARDALLOCATOR_H

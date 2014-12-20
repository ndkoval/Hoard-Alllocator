#ifndef BASE_SUPERBLOCK_H
#define BASE_SUPERBLOCK_H

#include <mutex>
#include <atomic>
#include <assert.h>
#include "hoard_constants.h"
#include "BaseHeap.h"
#include "BaseSuperblockHeader.h"

namespace hoard {

template<size_t kSuperblockSize>
class BaseSuperblock {
	using header_t = BaseSuperblockHeader<kSuperblockSize>;

public:
	BaseSuperblock() {
	};

	static BaseSuperblock<kSuperblockSize> *Get(void *ptr) {
		return reinterpret_cast<BaseSuperblock<kSuperblockSize> *>(reinterpret_cast<size_t>(ptr) & (kSuperblockSize - 1));
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

using Superblock = BaseSuperblock<kSuperblockSize>;
static_assert(sizeof(Superblock) == kSuperblockSize, "ivalid superblock struct size");
}
#endif // BASE_SUPERBLOCK_H
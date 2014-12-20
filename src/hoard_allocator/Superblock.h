#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <mutex>
#include <atomic>
#include <assert.h>
#include "hoard_constants.h"
#include "BaseHeap.h"
#include "SuperblockHeader.h"

namespace hoard {

template<size_t kSuperblockSize>
class Superblock {

public:
	Superblock() {
	};

	static Superblock<kSuperblockSize> *Get(void *ptr) {
		return reinterpret_cast<Superblock<kSuperblockSize> *>(reinterpret_cast<size_t>(ptr) & (kSuperblockSize - 1));
	}

private:
	SuperblockHeader<kSuperblockSize> header_;
	constexpr static size_t kDataSize = kSuperblockSize - sizeof(SuperblockHeader<kSuperblockSize>);
	char data[kDataSize];

public:
	SuperblockHeader<kSuperblockSize> &header() {
		return header_;
	};

};

static_assert(sizeof(Superblock<kSuperblockSize>) == kSuperblockSize, "ivalid superblock struct size");
}
#endif // SUPERBLOCK_H

#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <mutex>
#include <atomic>
#include <cassert>
#include "hoard_constants.h"
#include "BaseHeap.h"
#include "SuperblockHeader.h"

namespace hoard {

class Superblock {

public:
	Superblock() {
    assert(reinterpret_cast<size_t>(this) % kSuperblockSize == 0 && "Not alligned superblock");
  };
  
	Superblock(Superblock const &) = delete;
	Superblock & operator=(const Superblock &) = delete;

	static Superblock *Get(void *ptr) {
		return reinterpret_cast<Superblock *>(reinterpret_cast<size_t>(ptr) & ~(kSuperblockSize - 1));
	}

  static Superblock *Make() {
    return new (mmapAligned(kSuperblockSize, kSuperblockSize)) Superblock();
  }

  static Superblock *Make(size_t kBlockSize) {
    auto res = Make();
    res->header().Init(kBlockSize);
    return res;
  }

  static void Destroy(Superblock * superblock )  {
    check_fatal(munmap(superblock, sizeof(Superblock)) == 0, "Superblock Destroy");
  }

	SuperblockHeader &header() {
		return header_;
	};

  bool operator==(const Superblock & other) const {
    return this == &other;
  }
  bool operator!=(const Superblock & other) const {
    return !(*this == other);
  }

private:
	SuperblockHeader header_;
	constexpr static size_t kDataSize = kSuperblockSize - sizeof(SuperblockHeader);
	char data[kDataSize];
};

static_assert(sizeof(Superblock) == kSuperblockSize, "ivalid superblock struct size");
}
#endif // SUPERBLOCK_H

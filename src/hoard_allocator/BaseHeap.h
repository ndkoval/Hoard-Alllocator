#ifndef BASE_HEAP_H
#define BASE_HEAP_H

#include <mutex>
#include <hoard_constants.h>

namespace hoard {

//Superblock.h
template<size_t kSuperblockSize>
class Superblock;

template<size_t kSuperblockSize>
class BaseHeap {
public:

	lock_t lock() {
		return lock_;
	}

//	virtual Superblock<kSuperblockSize> *getSuperblock() = 0;
//	virtual void addSuperblock(Superblock<kSuperblockSize> *superblock) = 0;

	bool operator==(const BaseHeap &another) {
		return this == &another;
	}

protected:
	lock_t lock_;
};

}

#endif // BASE_HEAP_H

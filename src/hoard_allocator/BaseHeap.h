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

	lock_t lock_;

	virtual Superblock<kSuperblockSize> *GetSuperblock() = 0;
	virtual void AddSuperblock(Superblock<kSuperblockSize> *superblock) = 0;

	bool operator==(const BaseHeap &another) {
		return this == &another;
	}
};

}
#endif // BASE_HEAP_H

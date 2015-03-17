#ifndef BASE_HEAP_H
#define BASE_HEAP_H

#include <mutex>
#include <hoard_constants.h>

namespace hoard {

//Superblock.h
class Superblock;

class BaseHeap {
public:

	lock_t lock;
	BaseHeap(){};
	BaseHeap(const BaseHeap &) = delete;
	BaseHeap & operator=(const BaseHeap &) = delete;

	virtual void OnFreeSuperblock(Superblock *superblock) = 0;
  virtual size_t one_block_size() const = 0; //TODO : push field "one_block_size_" to BaseHeap, and make method non-virtual

	bool operator==(const BaseHeap &another) const {
		return this == &another;
	}
};

}
#endif // BASE_HEAP_H

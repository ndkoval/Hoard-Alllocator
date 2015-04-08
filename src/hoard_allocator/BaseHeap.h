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
	BaseHeap(size_t one_block_size) : one_block_size_(one_block_size){};
	BaseHeap(const BaseHeap &) = delete;
	BaseHeap & operator=(const BaseHeap &) = delete;


	bool operator==(const BaseHeap &another) const {
		return this == &another;
	}

  virtual ~BaseHeap(){};

  size_t one_block_size() const {
    return one_block_size_;
  }

  virtual void Free(Superblock* superblock, void* ptr) = 0;

  const size_t one_block_size_;

};

}
#endif // BASE_HEAP_H

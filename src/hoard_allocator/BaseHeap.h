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

  void Free(Superblock * superblock, void * ptr);

	bool operator==(const BaseHeap &another) const {
		return this == &another;
	}

  virtual ~BaseHeap(){};

  size_t one_block_size() const {
    return one_block_size_;
  }

protected:
  virtual void OnFreeSuperblock(Superblock *superblock) = 0;

  const size_t one_block_size_;

};

}
#endif // BASE_HEAP_H

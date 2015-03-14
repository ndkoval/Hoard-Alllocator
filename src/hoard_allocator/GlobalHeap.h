#ifndef GLOBAL_HEAP_H
#define GLOBAL_HEAP_H

#include "BaseHeap.h"
#include "Superblock.h"
#include "FreeSuperblockManager.h"

namespace hoard {

class GlobalHeap : public BaseHeap {
public:
	GlobalHeap(FreeSuperblockManager & parent_heap, size_t block_size) :
			free_superblock_manager_(parent_heap),
			one_block_size_(block_size) {
    trace("GlobalHeap: ", this, ". Construct.");
	}

	Superblock *GetSuperblock() { // callee must take lock
    trace("GlobalHeap: ", this, ". GetSuperblock");
		if (superblock_stack_.IsEmpty()) {
			Superblock * result = free_superblock_manager_.GetSuperblock();
			result->header().Init(one_block_size_);
			result->header().set_owner(this);
			return result;
		} else {
			Superblock *superblock = superblock_stack_.Pop();
			return superblock;
		}
	}

	void AddSuperblock(Superblock *superblock) { // callee must take lock
    trace("GlobalHeap: ", this, ". AddSuperblock: ", superblock);
		assert(superblock->header().one_block_size() == one_block_size_);
		if (superblock->header().empty()) {
      trace("empty");
			superblock->header().set_owner(nullptr);
			free_superblock_manager_.AddSuperblock(superblock);
		} else {
      trace("nonempty");
			superblock->header().set_owner(this);
			superblock_stack_.Push(superblock);
		}
	}

	virtual void OnFreeSuperblock(Superblock *superblock) override {
    trace("GlobalHeap: ", this, ". OnFreeSuperblock: ", superblock);
		assert(superblock->header().owner() == this);
		if (superblock->header().empty()) {
      trace("Superblock: ", superblock, " is empty");
			superblock_stack_.Remove(superblock);
			free_superblock_manager_.AddSuperblock(superblock);
		}
	}

	size_t one_block_size() const {
		return one_block_size_;
	}

private:
	FreeSuperblockManager & free_superblock_manager_;
	SuperblockStack superblock_stack_;
  const size_t one_block_size_;
};

}
#endif // GLOBAL_HEAP_H

#ifndef GLOBAL_HEAP_H
#define GLOBAL_HEAP_H

#include "BaseHeap.h"
#include "Superblock.h"
#include "FreeSuperblockManager.h"

namespace hoard {

class GlobalHeap : public BaseHeap {
public:
	GlobalHeap(FreeSuperblockManager & parent_heap, size_t one_block_size) :
      BaseHeap(one_block_size),
			free_superblock_manager_(parent_heap) {
    trace("GlobalHeap: ", this, ". Construct.");
	}

	Superblock *GetSuperblock() { // callee must take lock
    Superblock * result = nullptr;
		if (superblock_stack_.IsEmpty()) {
			result = free_superblock_manager_.GetSuperblock();
			result->header().Init(one_block_size_);
			result->header().set_owner(this);
			return result;
		} else {
			result = superblock_stack_.Pop();
		}
    trace("GlobalHeap: ", this, ". GetSuperblock: ", result);
    return result;
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

  void CheckInvariantsOrDie() {
    superblock_stack_.CheckInvariantsOrDie();
  }

  size_t superblock_count() {
    return superblock_stack_.Size();
  }

protected:
	virtual void OnFreeSuperblock(Superblock *superblock) override {
    trace("GlobalHeap: ", this, ". OnFreeSuperblock: ", superblock);
		assert(superblock->header().owner() == this);
		if (superblock->header().empty()) {
      trace("Superblock: ", superblock, " is empty");
			superblock_stack_.Remove(superblock);
			free_superblock_manager_.AddSuperblock(superblock);
		}
	}

private:
	FreeSuperblockManager & free_superblock_manager_;
	SuperblockStack superblock_stack_;
};

}
#endif // GLOBAL_HEAP_H

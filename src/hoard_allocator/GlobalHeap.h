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
			block_size_(block_size) {
	}

	Superblock *GetSuperblock() { // callee must take lock
		if (superblock_stack_.IsEmpty()) {
			Superblock * result = free_superblock_manager_.GetSuperblock();
			result->header().Init(block_size_);
			result->header().set_owner(this);
			return result;
		} else {
			Superblock *superblock = superblock_stack_.Pop();
			return superblock;
		}
	}

	void AddSuperblock(Superblock *superblock) { // callee must take lock
		assert(superblock->header().block_size() == block_size_);
		if (superblock->header().empty()) {
			superblock->header().set_owner(nullptr);
			free_superblock_manager_.AddSuperblock(superblock);
		} else {
			superblock->header().set_owner(this);
			superblock_stack_.Push(superblock);
		}
	}

	virtual void OnFreeSuperblock(Superblock *superblock) override {
		assert(superblock->header().owner() == this);
		if (superblock->header().empty()) {
			superblock_stack_.Remove(superblock);
			free_superblock_manager_.AddSuperblock(superblock);
		}
	}

	size_t block_size() const {
		return block_size_;
	}

private:
	FreeSuperblockManager & free_superblock_manager_;
	SuperblockStack superblock_stack_;
	const size_t block_size_;
};

}
#endif // GLOBAL_HEAP_H

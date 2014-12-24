#ifndef GLOBAL_HEAP_H
#define GLOBAL_HEAP_H

#include "BaseHeap.h"
#include "Superblock.h"

namespace hoard {


class GlobalHeap : public BaseHeap {
public:
	GlobalHeap(FreeSuperblockManager *parent_heap, size_t block_size) :
			free_superblock_manager_(parent_heap),
			block_size_(block_size) {
	}

	Superblock *GetSuperblock() {
		lock_guard guard(BaseHeap::lock);
		if (superblock_stack_.IsEmpty()) {
			return free_superblock_manager_->GetSuperblock();
		} else {
			return superblock_stack_.Pop();
		}
	}

	void AddSuperblock(Superblock *superblock) {
		lock_guard guard(BaseHeap::lock);
		if (superblock->header().IsFree()) {
			free_superblock_manager_->AddSuperblock(superblock);
		} else {
			superblock_stack_.Push(superblock);
		}
	}


	virtual void OnFreeSuperblock(Superblock *superblock) override {

	}

private:
	FreeSuperblockManager *free_superblock_manager_;
	SuperblockStack superblock_stack_;
	size_t block_size_;
};

}
#endif // GLOBAL_HEAP_H

#ifndef GLOBAL_HEAP_H
#define GLOBAL_HEAP_H

#include "BaseHeap.h"
#include "Superblock.h"

namespace hoard {


class GlobalHeap : public BaseHeap {
public:
	GlobalHeap(FreeSuperblockManager *parentHeap) {
		freeSuperblockManager_ = parentHeap;
	}

	Superblock *GetSuperblock() {
		lock_guard guard(BaseHeap::lock);
		if (superblockStack_.IsEmpty()) {
			return freeSuperblockManager_->GetSuperblock();
		} else {
			return superblockStack_.Pop();
		}
	}

	void AddSuperblock(Superblock *superblock) {
		lock_guard guard(BaseHeap::lock);
		if (superblock->header().IsFree()) {
			freeSuperblockManager_->AddSuperblock(superblock);
		} else {
			superblockStack_.Push(superblock);
		}
	}


	virtual void OnFreeSuperblock(Superblock *superblock) override {

	}

private:
	FreeSuperblockManager *freeSuperblockManager_;
	SuperblockStack superblockStack_;
};

}
#endif // GLOBAL_HEAP_H

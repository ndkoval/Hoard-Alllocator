#ifndef GLOBAL_HEAP_H
#define GLOBAL_HEAP_H

#include "BaseHeap.h"
#include "Superblock.h"

namespace hoard {

template<size_t kSuperblockSize>
class GlobalHeap : public BaseHeap<kSuperblockSize> {
public:
	GlobalHeap(BaseHeap<kSuperblockSize> *parentHeap) {
		parentHeap_ = parentHeap;
	}

	Superblock<kSuperblockSize> *GetSuperblock() override {
		lock_guard guard(BaseHeap<kSuperblockSize>::lock_);
		if (superblockStack.IsEmpty()) {
			return parentHeap_->GetSuperblock();
		} else {
			return superblockStack.Pop();
		}
	}

	void AddSuperblock(Superblock<kSuperblockSize> *superblock) override {
		lock_guard guard(BaseHeap<kSuperblockSize>::lock_);
		if (superblock->header().IsFree()) {
			parentHeap_->AddSuperblock(superblock);
		} else {
			superblockStack.Push(superblock);
		}
	}

private:
	BaseHeap<kSuperblockSize> *parentHeap_;
	SuperblockStack<kSuperblockSize> superblockStack;
};

}
#endif // GLOBAL_HEAP_H

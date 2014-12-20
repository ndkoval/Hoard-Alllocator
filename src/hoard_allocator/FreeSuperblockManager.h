#ifndef FREE_SUPERBLOCK_MANAGER_H
#define FREE_SUPERBLOCK_MANAGER_H

#include <utils.h>
#include "BaseHeap.h"
#include "Superblock.h"
#include "SuperblockStack.h"

namespace hoard {

template<size_t kSuperblockSize>
class FreeSuperblockManager : virtual BaseHeap<kSuperblockSize> {

public:
	FreeSuperblockManager() : superblock_count_(0) {
		MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
	}

	virtual ~FreeSuperblockManager() {
		lock_guard guard(BaseHeap<kSuperblockSize>::lock_);
		while (!superblockStack.IsEmpty()) {
			Superblock<kSuperblockSize> *ptr = superblockStack.Pop();
			munmap(ptr, sizeof(Superblock<kSuperblockSize>));
		}
	}

	Superblock<kSuperblockSize> *GetSuperblock() override {
		lock_guard guard(BaseHeap<kSuperblockSize>::lock_);
		if (superblockStack.IsEmpty()) {
			MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
		}
		superblock_count_--;

		return superblockStack.Pop();
	}

	void AddSuperblock(Superblock<kSuperblockSize> *superblock) override {
		lock_guard guard(BaseHeap<kSuperblockSize>::lock_);

		if (superblock_count_ < kMaxFreeSuperblocks) {
			superblockStack.Push(superblock);
			superblock_count_++;
		} else {
			munmap(superblock, sizeof(superblock));
		}
	}

private:
	SuperblockStack<kSuperblockSize> superblockStack;
	size_t superblock_count_;

	void MapNewSuperblocks(size_t count) {
		char *newSuperBlocksMemory = (char *) mmapAnonymous(count * sizeof(Superblock<kSuperblockSize>));
		for (int i = 0; i < count; i++, newSuperBlocksMemory += sizeof(Superblock<kSuperblockSize>)) {
			superblockStack.Push(new(newSuperBlocksMemory) Superblock<kSuperblockSize>);
		}
		superblock_count_ += count;
	}
};

}
#endif // FREE_SUPERBLOCK_MANAGER_H

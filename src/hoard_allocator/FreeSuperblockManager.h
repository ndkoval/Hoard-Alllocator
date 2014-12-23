#ifndef FREE_SUPERBLOCK_MANAGER_H
#define FREE_SUPERBLOCK_MANAGER_H

#include <utils.h>
#include "BaseHeap.h"
#include "Superblock.h"
#include "SuperblockStack.h"

namespace hoard {

class FreeSuperblockManager {

public:
	FreeSuperblockManager() : superblock_count_(0) {
		MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
	}

	virtual ~FreeSuperblockManager() {
		lock_guard guard(lock_);
		while (!superblockStack.IsEmpty()) {
			Superblock *ptr = superblockStack.Pop();
			munmap(ptr, sizeof(Superblock));
		}
	}

	Superblock *GetSuperblock() {
		lock_guard guard(lock_);
		if (superblockStack.IsEmpty()) {
			MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
		}
		superblock_count_--;

		return superblockStack.Pop();
	}

	void AddSuperblock(Superblock *superblock) {
		lock_guard guard(lock_);

		if (superblock_count_ < kMaxFreeSuperblocks) {
			superblockStack.Push(superblock);
			superblock_count_++;
		} else {
			munmap(superblock, sizeof(superblock));
		}
	}

private:
	SuperblockStack superblockStack;
	size_t superblock_count_;
	lock_t lock_;

	void MapNewSuperblocks(size_t count) {
		char *newSuperBlocksMemory = (char *) mmapAnonymous(count * sizeof(Superblock));
		for (size_t i = 0; i < count; i++, newSuperBlocksMemory += sizeof(Superblock)) {
			superblockStack.Push(new(newSuperBlocksMemory) Superblock);
		}
		superblock_count_ += count;
	}
};

}
#endif // FREE_SUPERBLOCK_MANAGER_H

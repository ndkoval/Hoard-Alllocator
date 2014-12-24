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
		while (!superblock_stack_.IsEmpty()) {
			Superblock *ptr = superblock_stack_.Pop();
			munmap(ptr, sizeof(Superblock));
		}
	}

	Superblock *GetSuperblock() {
		lock_guard guard(lock_);
		if (superblock_stack_.IsEmpty()) {
			MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
		}
		superblock_count_--;

		return superblock_stack_.Pop();
	}

	void AddSuperblock(Superblock *superblock) {
		lock_guard guard(lock_);

		if (superblock_count_ < kMaxFreeSuperblocks) {
			superblock_stack_.Push(superblock);
			superblock_count_++;
		} else {
			munmap(superblock, sizeof(superblock));
		}
	}

private:
	SuperblockStack superblock_stack_;
	size_t superblock_count_;
	lock_t lock_;

	void MapNewSuperblocks(size_t count) {
		Superblock *newSuperBlocksMemory = reinterpret_cast<Superblock *>(mmapAnonymous(count * sizeof(Superblock), sizeof(Superblock)));
		for (size_t i = 0; i < count; i++, newSuperBlocksMemory++) {
			superblock_stack_.Push(new(newSuperBlocksMemory) Superblock);
		}
		superblock_count_ += count;
	}
};

}
#endif // FREE_SUPERBLOCK_MANAGER_H

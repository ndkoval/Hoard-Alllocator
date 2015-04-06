#ifndef FREE_SUPERBLOCK_MANAGER_H
#define FREE_SUPERBLOCK_MANAGER_H

#include <utils.h>
#include "BaseHeap.h"
#include "Superblock.h"
#include "SuperblockStack.h"

namespace hoard {

class FreeSuperblockManager {

public:
	FreeSuperblockManager()
//      : superblock_count_(0)
  {
		MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
    trace("SuperblockManager: ", "Construct");
	}

	FreeSuperblockManager(const FreeSuperblockManager &) = delete;
	FreeSuperblockManager & operator=(const FreeSuperblockManager &) = delete;

	virtual ~FreeSuperblockManager() {
    trace("SuperblockManager: ", "Destruct");
		lock_guard guard(lock_);
    superblock_alive_ -= superblock_stack_.Size();
		while (!superblock_stack_.IsEmpty()) {
			Superblock *ptr = superblock_stack_.Pop();
			munmap(ptr, sizeof(Superblock));
		}
	}

	Superblock *GetSuperblock() {
    trace("SuperblockManager: ", "GetSuperblock");
		lock_guard guard(lock_);
		if (superblock_stack_.IsEmpty()) {
			MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
		}
//		superblock_count_--;

		return superblock_stack_.Pop();
	}

	void AddSuperblock(Superblock *superblock) {
    trace("SuperblockManager: ", "AddSuperblock: ", superblock);
		lock_guard guard(lock_);
    superblock->header().set_owner(nullptr);

		if (superblock_count() < kMaxFreeSuperblocks) {
      trace("SuperblockManager: ", "Superblock Saved: ", superblock);
			superblock_stack_.Push(superblock);
//			superblock_count_++;
		} else {
      trace("SuperblockManager: ", "Superblock Destroyed: ", superblock);
      --superblock_alive_;
			munmap(superblock, sizeof(Superblock));
		}
    trace("SuperblockManager: superblocks_alive: ", superblock_alive_);
	}

private:
	SuperblockStack superblock_stack_;
  size_t superblock_count() {
    return superblock_stack_.Size();
  }
	//size_t superblock_count_;
	lock_t lock_;

  size_t superblock_alive_ = 0;

	void MapNewSuperblocks(size_t count) {
    trace("SuperblockManager: ", "MapNewSuperblocks: ", count);
		Superblock *newSuperBlocksMemory = reinterpret_cast<Superblock *>(mmapAnonymous(count * sizeof(Superblock), sizeof(Superblock)));
		for (size_t i = 0; i < count; i++, newSuperBlocksMemory++) {
			superblock_stack_.Push(new(newSuperBlocksMemory) Superblock);
		}
//		superblock_count_ += count;
		superblock_alive_ += count;
    trace("SuperblockManager: superblocks_alive: ", superblock_alive_);
	}
};

}
#endif // FREE_SUPERBLOCK_MANAGER_H

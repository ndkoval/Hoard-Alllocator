#ifndef FREE_SUPERBLOCK_MANAGER_H
#define FREE_SUPERBLOCK_MANAGER_H

#include <utils.h>
#include "BaseHeap.h"
#include "Superblock.h"

namespace hoard {

template<size_t kSuperblockSize>
class FreeSuperblockManager {

public:
	FreeSuperblockManager() : superblock_count_(0) {
		MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
	}

	Superblock<kSuperblockSize> *GetSuperblock() {
		hoard::lock_guard guard(lock_);
		if (superblockStack.empty()) {
			MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
		}
		superblock_count_--;

		return superblockStack.Pop();
	}

	void ReturnSuperblock(Superblock<kSuperblockSize> *superblock) {
		hoard::lock_guard guard(lock_);

		if (superblock_count_ < kMaxFreeSuperblocks) {
			superblockStack.Push(superblock);
			superblock_count_++;
		} else {
			munmap(superblock, sizeof(superblock));
		}
	}

private:
	struct FreeSuperblockStack {
	private:
		Superblock<kSuperblockSize> *head_;
	public:
		FreeSuperblockStack() : head_(nullptr) {
		}

		bool empty() {
			return head_ == nullptr;
		}

		void Push(Superblock<kSuperblockSize> *superblock) {
			superblock->header().setNext(head_);
			head_ = superblock;
		}

		Superblock<kSuperblockSize> *Pop() {
			assert(!empty());
			Superblock<kSuperblockSize> *result = head_;
			head_ = head_->header().next();
			return result;
		}
	};

	FreeSuperblockStack superblockStack;
	size_t superblock_count_;

	void MapNewSuperblocks(size_t count) {
		char *newSuperBlocksMemory = (char *) mmapAnonymous(count * sizeof(Superblock<kSuperblockSize>));
		for (int i = 0; i < count; i++, newSuperBlocksMemory += sizeof(Superblock<kSuperblockSize>)) {
			superblockStack.Push(new(newSuperBlocksMemory) Superblock<kSuperblockSize>);
		}
		superblock_count_ += count;
	}

	lock_t lock_;
};

}
#endif // FREE_SUPERBLOCK_MANAGER_H

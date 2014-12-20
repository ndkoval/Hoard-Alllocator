#ifndef FREE_SUPERBLOCK_MANAGER_H
#define FREE_SUPERBLOCK_MANAGER_H

#include "BaseHeap.h"
#include "BaseSuperblock.h"

namespace hoard {

template<size_t kSuperblockSize>
class BaseFreeSuperblockManager : public BaseHeap {

public:
	BaseFreeSuperblockManager() {
		MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
	}

	BaseSuperblock<kSuperblockSize> *GetSuperblock() {
		hoard::lock_guard guard(lock_);
		if (superblockStack.empty()) {
			MapNewSuperblocks(kDefaultMapNewSuperblocksCount);
		}
		superblock_count_--;

		return superblockStack.Pop();
	}

	void ReturnSuperblock(BaseSuperblock<kSuperblockSize> *superblock) {
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
		BaseSuperblock<kSuperblockSize> *head_;
	public:
		FreeSuperblockStack() : head_(nullptr) {
		}

		bool empty() {
			return head_ == nullptr;
		}

		void Push(BaseSuperblock<kSuperblockSize> *superblock) {
			superblock->header().setNext(head_);
			head_ = superblock;
		}

		BaseSuperblock<kSuperblockSize> *Pop() {
			assert(!empty());
			BaseSuperblock<kSuperblockSize> *result = head_;
			head_ = head_->header().next();
			return result;
		}
	};

	hoard::lock_t lock_;
	FreeSuperblockStack superblockStack;
	size_t superblock_count_;

	void MapNewSuperblocks(size_t count) {
		char *newSuperBlocksMemory = (char *) mmapAnonymous(count * sizeof(BaseSuperblock<kSuperblockSize>));
		for (int i = 0; i < count; i++, newSuperBlocksMemory += sizeof(BaseSuperblock<kSuperblockSize>)) {
			superblockStack.Push(new(newSuperBlocksMemory) BaseSuperblock<kSuperblockSize>);
		}
		superblock_count_ += count;
	}

};

using FreeSuperblockManager = BaseFreeSuperblockManager<hoard::kSuperblockSize>;

}
#endif // FREE_SUPERBLOCK_MANAGER_H

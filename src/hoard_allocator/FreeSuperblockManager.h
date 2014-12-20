#ifndef FREE_SUPERBLOCK_MANAGER_H
#define FREE_SUPERBLOCK_MANAGER_H

#include "BaseHeap.h"
#include "BaseSuperblock.h"

namespace hoard {

template<size_t kSuperblockSize>
class BaseFreeSuperblockManager : public BaseHeap {

public:
	BaseFreeSuperblockManager() {
		//TODO
	}

	Superblock *GetSuperblock() {
		//TODO
		return nullptr;
	}

	void ReturnSuperblock() {
		//TODO
	}

	hoard::lock_t lock() {
		return lock_;
	}

private:
	hoard::lock_t lock_;
	Superblock* superblock_list_;
	size_t superblock_count_;

	void MapNewSuperblocks(size_t count) {

	}


};

using FreeSuperblockManager = BaseFreeSuperblockManager<hoard::kSuperblockSize>;

}
#endif // FREE_SUPERBLOCK_MANAGER_H

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

	virtual ~GlobalHeap() {
	}


	Superblock<kSuperblockSize> *GetSuperblock() override {
		return nullptr;
	}

	void AddSuperblock(Superblock<kSuperblockSize> *superblock) override {

	}

private:
	BaseHeap<kSuperblockSize> *parentHeap_;
};

}


#endif // GLOBAL_HEAP_H

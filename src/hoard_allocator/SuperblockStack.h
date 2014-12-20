#ifndef SUPERBLOCK_STACK_H
#define SUPERBLOCK_STACK_H

#include "Superblock.h"
#include "BaseStack.h"

namespace hoard {

template<size_t kSuperblockSize>
class SuperblockStack : public BaseStack<Superblock<kSuperblockSize> > {

protected:
	virtual void SetNext(Superblock<kSuperblockSize> *superblock, Superblock<kSuperblockSize> *value) override {
		superblock->header().setNext(value);
	}

	virtual void SetPrev(Superblock<kSuperblockSize> *superblock, Superblock<kSuperblockSize> *value) override {
		superblock->header().setPrev(value);
	}

	virtual Superblock<kSuperblockSize> *Next(Superblock<kSuperblockSize> *superblock) override {
		return superblock->header().next();
	}
};

}

#endif // SUPERBLOCK_STACK_H
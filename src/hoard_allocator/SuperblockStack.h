#ifndef SUPERBLOCK_STACK_H
#define SUPERBLOCK_STACK_H

#include "Superblock.h"
#include "BaseStack.h"

namespace hoard {


class SuperblockStack : public BaseStack<Superblock > {

protected:
	virtual void SetNext(Superblock *superblock, Superblock *value) override {
		superblock->header().setNext(value);
	}

	virtual void SetPrev(Superblock *superblock, Superblock *value) override {
		superblock->header().setPrev(value);
	}

	virtual Superblock *Next(Superblock *superblock) override {
		return superblock->header().next();
	}
};

}
#endif // SUPERBLOCK_STACK_H
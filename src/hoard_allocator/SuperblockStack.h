#ifndef SUPERBLOCK_STACK_H
#define SUPERBLOCK_STACK_H

#include "Superblock.h"
#include "BaseStack.h"

namespace hoard {


class SuperblockStack : public BaseStack<Superblock> {

public:
	void Remove(Superblock *superblock) {
		Superblock * curent = head_;
		while (curent != nullptr || curent != superblock) {
			curent = Next(head_);
		}
		assert(curent != nullptr && "no such element in Stack");
		Superblock * prev = Prev(superblock);
		Superblock *next = Next(superblock);
		if(prev) {
			SetNext(prev, next);
		}
		if(next) {
			SetPrev(next, prev);
		}
		SetNext(superblock, nullptr);
		SetPrev(superblock, nullptr);
	}




protected:
	virtual Superblock *Next(Superblock *superblock) override {
		return superblock->header().next();
	}

	virtual Superblock *Prev(Superblock *superblock) {
		return superblock->header().prev();
	}

	virtual void SetNext(Superblock *superblock, Superblock *value) override {
		superblock->header().set_next(value);
	}

	virtual void SetPrev(Superblock *superblock, Superblock *value) override {
		superblock->header().set_prev(value);
	}

};

}
#endif // SUPERBLOCK_STACK_H
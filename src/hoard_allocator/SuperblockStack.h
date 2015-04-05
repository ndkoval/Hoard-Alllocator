#ifndef SUPERBLOCK_STACK_H
#define SUPERBLOCK_STACK_H

#include "Superblock.h"
#include "BaseStack.h"

namespace hoard {


class SuperblockStack : public BaseStack<Superblock> {

public:
	void Remove(Superblock *superblock) {
    trace("stack remove");
#ifndef NDEBUG
    // O(n) check, that superblock is in this stack
    assert(Contains(superblock) && "no such element in Stack");
#endif //debug
		Superblock *prev = Prev(superblock);
		Superblock *next = Next(superblock);

    if (head_ == superblock) {
      Pop();
      return;
    }

		if(prev != nullptr) {
			SetNext(prev, next);
		}

		if(next != nullptr) {
			SetPrev(next, prev);
		}

    --size_;

    superblock->header().set_next(nullptr);
    superblock->header().set_prev(nullptr);

    CheckInvariantsOrDie();
    trace("stack after remove");

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
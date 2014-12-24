#ifndef BLOCK_STACK_H
#define BLOCK_STACK_H

#include "Block.h"
#include "BaseStack.h"

namespace hoard {

class BlockStack : public BaseStack<Block> {

protected:
	virtual void SetNext(Block *block, Block *value) override {
		block->set_next(value);
	}

	virtual void SetPrev(Block *block, Block *value) override {
		// not needed
	}

	virtual Block *Next(Block *block) override {
		return block->next();
	}
};

}
#endif // BLOCK_STACK_H
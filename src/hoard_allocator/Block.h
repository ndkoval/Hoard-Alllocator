#ifndef BLOCK_H
#define BLOCK_H

#include <hoard_constants.h>

namespace hoard {

class Block {

public:
	Block *next() const {
		return next_;
	}

	void set_next(Block *next) {
		next_ = next;
	}

private:
	Block *next_;
};

static_assert(sizeof(Block) >= kMinBlockSize, "size of Block struct should be more of equal than MIN_SIZE");

}
#endif // BLOCK_H
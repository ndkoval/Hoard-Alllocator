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

  void MakeMagic() { //for block corruption test
    set_next(GetMagicNext());
  }

  bool IsMagic() { //for block corruption test
    return next_ == GetMagicNext();
  }



private:
	Block *next_;
  Block * GetMagicNext() {
    return reinterpret_cast<Block *> (reinterpret_cast<size_t>(this) ^ kMagicNumber);
  }
};

static_assert(sizeof(Block) >= kMinBlockSize, "size of Block struct should be more of equal than MIN_SIZE");

}
#endif // BLOCK_H
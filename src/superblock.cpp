
#include "superblock.h"

    void hoard::FreeBlockStack::push(void *spaceForExtraBlock) {
        _head = new (spaceForExtraBlock)  Block; // constructs new Block in free space
    }
    void * hoard::FreeBlockStack::pop() {
        assert(!empty());
        Block * result = _head;
        _head = _head->next;
        return reinterpret_cast<void *> (result);
    }

    bool hoard::FreeBlockStack::empty() {
        return _head == nullptr;
    }

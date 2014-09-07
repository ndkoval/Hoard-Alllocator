
#include "superblock.h"

    void hoard::free_block_stack::push(void * space_for_extra_block) {
        _head = new (space_for_extra_block)  block; // constructs new block in free space
    }
    void * hoard::free_block_stack::pop() {
        assert(!empty());
        block * result = _head;
        _head = _head->next;
        return reinterpret_cast<void *> (result);
    }

    bool hoard::free_block_stack::empty() {
        return _head == nullptr;
    }

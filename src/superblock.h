#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include "hoard_constants.h"

namespace hoard {



struct block {
    block * next;
};

static_assert(sizeof(block) >= MIN_BLOCK_SIZE, "size of block struct should be more of equal than MIN_SIZE");


struct free_block_stack {
public:
    void push(void * space_for_extra_block);
    void * pop();
    bool empty();
private:
    block * _head;
};



template<size_t superblock_size>
class superblock_header {
public:
    std::mutex lock;
    
    base_superblock<superblock_size> * prev, next;
private:
    size_t _block_size; // power of 2
    char * _buffer_start; // superblock buffer position
    char * _blocks_data_start; // first block start. Aligned by block size

    



};

template<size_t superblock_size>

    class base_superblock
    {
    public:
        superblock_header<superblock_size> header;
        constexpr static unsigned BUFF_SIZE = superblock_size - sizeof(superblock_header);
        char buff[BUFF_SIZE];

        Superblock();

    };
}

using superblock = BaseSuperBlock<SUPERBLOCK_SIZE>;

static_assert(sizeof(superblock) == SUPERBLOCK_SIZE, "ivalid superblock struct size")
#endif // SUPERBLOCK_H

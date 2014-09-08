#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <mutex>
#include <assert.h>
#include "HoardConstants.h"

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
class base_superblock;



template<size_t superblock_size>
class superblock_header {
public:
    std::mutex lock;

    base_superblock<superblock_size> * prev;
    base_superblock<superblock_size> * next;
private:
    size_t _block_size; // power of 2
    char * _buffer_start; // superblock buffer position
    char * _blocks_data_start; // first block start. Aligned by block size


};

template<size_t superblock_size>


class base_superblock
{
    using header_t = superblock_header<superblock_size>;

public:
    superblock_header<superblock_size> header;
    constexpr static unsigned BUFF_SIZE = superblock_size - sizeof(header_t);
    char buff[BUFF_SIZE];

    base_superblock();

};

using superblock = base_superblock<SUPERBLOCK_SIZE>;
static_assert(sizeof(superblock) == SUPERBLOCK_SIZE, "ivalid superblock struct size");
}
#endif // SUPERBLOCK_H

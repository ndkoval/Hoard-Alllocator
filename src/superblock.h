#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <mutex>
#include <assert.h>
#include "HoardConstants.h"

namespace hoard {

    struct Block {
        Block *next;
    };

    static_assert(sizeof(Block) >= MIN_BLOCK_SIZE, "size of Block struct should be more of equal than MIN_SIZE");


    struct FreeBlockStack {
    public:
        void push(void *spaceForExtraBlock);

        void *pop();

        bool empty();

    private:
        Block *_head;
    };

    template<size_t superblock_size>
    class BaseSuperblock;


    template<size_t superblock_size>
    class SuperblockHeader {
    public:
        std::mutex lock;

        BaseSuperblock<superblock_size> *prev;
        BaseSuperblock<superblock_size> *next;
    private:
        size_t _block_size; // power of 2
        char *_buffer_start; // superblock buffer position
        char *_blocks_data_start; // first Block start. Aligned by Block size


    };

    template<size_t superblockSize>


    class BaseSuperblock {
        using header_t = SuperblockHeader<superblockSize>;

    public:
        SuperblockHeader<superblockSize> header;
        constexpr static unsigned BUFF_SIZE = superblockSize - sizeof(header_t);
        char buff[BUFF_SIZE];

        BaseSuperblock();

    };

    using superblock = BaseSuperblock<SUPERBLOCK_SIZE>;
    static_assert(sizeof(superblock) == SUPERBLOCK_SIZE, "ivalid superblock struct size");
}
#endif // SUPERBLOCK_H

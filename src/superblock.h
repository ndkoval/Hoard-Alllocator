#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include "hoard_constants.h"

namespace hoard {


class superblock_header {



};

template<unsigned superblock_size>

    class base_superblock
    {
    public:
        superblock_header header;
        constexpr static unsigned BUFF_SIZE = superblock_size - sizeof(superblock_header);
        char buff[BUFF_SIZE];

        Superblock();

    };
}

using superblock = BaseSuperBlock<SUPERBLOCK_SIZE>;

static_assert(sizeof(superblock) == SUPERBLOCK_SIZE, "ivalid superblock struct size")
#endif // SUPERBLOCK_H

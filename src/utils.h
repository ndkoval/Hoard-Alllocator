#ifndef UTILS_H
#define UTILS_H

#include "hoard_constants.h"
#include <assert.h>

namespace hoard {

    bool is_power_of_2(size_t n)
    {
        return ((n != 0) && !(n & (n - 1)));
    }

    void * get_aligned_pages(size_t pages_num, size_t alignment) {
        assert(alignment > PAGE_SIZE);

        return NULL;
    }


    bool is_valid_alignment(size_t alignment)
    {
        if ((alignment % sizeof(void*)) != 0)
            return false;

        if (!is_power_of_2(alignment))
            return false;

        return true;
    }
}


#endif // UTILS_H

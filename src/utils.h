#ifndef UTILS_H
#define UTILS_H

#include "HoardConstants.h"
#include <assert.h>
#include<sys/mman.h>

namespace hoard {

    constexpr bool isPowerOf2(size_t n)
    {
        return ((n != 0) && !(n & (n - 1)));
    }


    bool isValidAlignment(size_t alignment)
    {
        if ((alignment % sizeof(void*)) != 0)
            return false;

        return isPowerOf2(alignment);
    }

    char* get_first_aligned_pointer(char* ptr, size_t alignment) {
        size_t offset = ((size_t) ptr) % alignment;
        return ptr + offset;
    }

    constexpr size_t round_up(size_t what, size_t to) {
        return (what + to - 1) / to * to;
    }

    void *mmapAnonymous(void *addr, size_t length) {
        mmap(addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    }

    void *mmapAnonymous(size_t length) {
        mmapAnonymous(NULL, length);
    }

}


#endif // UTILS_H

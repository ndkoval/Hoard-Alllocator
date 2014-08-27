#ifndef UTILS_H
#define UTILS_H

#include "hoard_constants.h"
#include <assert.h>
#include<sys/mman.h>

namespace hoard {

    constexpr bool is_power_of_2(size_t n)
    {
        return ((n != 0) && !(n & (n - 1)));
    }

   /* void * get_aligned_pages(size_t pages_num, size_t alignment) {
        assert(alignment > PAGE_SIZE);

        return NULL;
    } */



    bool is_valid_alignment(size_t alignment)
    {
        if ((alignment % sizeof(void*)) != 0)
            return false;

        return is_power_of_2(alignment);

    }

    char* get_first_aligned_pointer(char* ptr, size_t alignment) {
        size_t offset = ((size_t) ptr) % alignment;
        return ptr + offset;
    }

    constexpr size_t round_up(size_t what, size_t to) {
        return (what + to - 1) / to * to;
    }

    void * mmap_anonymous(void * addr, size_t length) {
        mmap(addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    }

    void * mmap_anonymous(size_t length) {
        mmap_anonymous(NULL, length);


    }

}


#endif // UTILS_H

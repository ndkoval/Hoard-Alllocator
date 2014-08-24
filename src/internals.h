#ifndef INTERNALS_H
#define INTERNALS_H

#include <cstdlib>

namespace hoard
{
    void* internal_alloc(size_t size, size_t alignment);
    void* internal_alloc(size_t size);
    void  internal_free(void* ptr);
    void* internal_realloc(void *ptr, size_t size);

    bool  is_valid_alignment(size_t alignment);

    void* small_alloc(size_t size);
    void* big_alloc(size_t size, size_t alignment);
    void  small_free(void* ptr);
    void  big_free(void* ptr);
}

#endif

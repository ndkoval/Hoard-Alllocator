#ifndef INTERNALS_H
#define INTERNALS_H

#include <cstdlib>
#include "HoardConstants.h"

namespace hoard {
    void *internalAlloc(size_t size, size_t alignment = DEFAULT_ALIGNMENT);

    void internalFree(void *ptr);

    void *internalRealloc(void *ptr, size_t size);

    bool isValidAlignment(size_t alignment);

    void *smallAlloc(size_t size);

    void *bigAlloc(size_t size, size_t alignment);

    void smallFree(void *ptr);

    void bigFree(void *ptr);
}

#endif

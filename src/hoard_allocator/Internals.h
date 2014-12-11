#ifndef INTERNALS_H
#define INTERNALS_H

#include <cstdlib>
#include "hoard_constants.h"
#include "AllocFreeHashMap.h"

namespace hoard {

void *InternalAlloc(size_t size, size_t alignment = DEFAULT_ALIGNMENT);

void InternalFree(void *ptr);

void *InternalRealloc(void *ptr, size_t size);

//    bool IsValidAlignment(size_t alignment);

void *SmallAlloc(size_t size);

void *BigAlloc(size_t size, size_t alignment);

void SmallFree(void *ptr);

void BigFree(void *ptr);
}

#endif

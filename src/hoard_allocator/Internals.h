#ifndef HOARD_HOARD_INTERNALS_H
#define HOARD_HOARD_INTERNALS_H

#include <cstdlib>
#include "hoard_constants.h"
#include "AllocFreeHashMap.h"

namespace hoard {

void *InternalAlloc(size_t size, size_t alignment = kDefaultAlignment);

void InternalFree(void *ptr);

void *InternalRealloc(void *ptr, size_t size);

void *SmallAlloc(size_t size);

void *BigAlloc(size_t size, size_t alignment = kPageSize);

void SmallFree(void *ptr);

void *SmallAlloc(size_t size, size_t alignment = kDefaultAlignment);

bool BigFree(void *ptr);

}

#endif // HOARD_HOARD_INTERNALS_H

#include <cstring>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <algorithm>

#include "utils.h"
#include "Internals.h"
#include "tracing.h"
#include "FreeSuperblockManager.h"
#include "GlobalHeap.h"
#include "Local_heap.h"
#include "superblock.h"
#include "AllocFreeHashmap.h"

namespace {
    std::mutex bigAllocMutex;
    hoard::AllocFreeHashmap bigAllocates;

    hoard::FreeSuperblockManager globalFreeSuperblockManager();

    hoard::GlobalHeap testGlobalHeap();

    hoard::Local_heap testLocalHeap();
}

void *hoard::internalAlloc(size_t size, size_t alignment) {
    assert (alignment <= size);
    assert (isValidAlignment(alignment));

    bigAlloc(size, alignment);
}

void hoard::internalFree(void *ptr) {
    bigFree(ptr);
}

void *hoard::smallAlloc(size_t size) {
    return NULL;
}

void *hoard::bigAlloc(size_t size, size_t alignment) {
    std::lock_guard<std::mutex> lock(bigAllocMutex);

    return NULL;
}

void hoard::smallFree(void *ptr) {
}

void hoard::bigFree(void *ptr) {
    std::lock_guard<std::mutex> lock(bigAllocMutex);
}

void *hoard::internalRealloc(void *ptr, size_t size) {
    return NULL;
}

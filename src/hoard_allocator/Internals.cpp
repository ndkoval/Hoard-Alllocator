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
#include "LocalHeap.h"
#include "superblock.h"
#include "AllocFreeHashMap.h"

namespace {
std::mutex bigAllocMutex;
hoard::AllocFreeHashMap bigAllocates;

hoard::FreeSuperblockManager globalFreeSuperblockManager;

hoard::GlobalHeap testGlobalHeap;

hoard::LocalHeap testLocalHeap;
}

void *hoard::InternalAlloc(size_t size, size_t alignment) {
  assert (alignment <= size);
  assert (IsValidAlignment(alignment));

  BigAlloc(size, alignment);
  //TODO
  return nullptr;
}

void hoard::InternalFree(void *ptr) {
  BigFree(ptr);
}

void *hoard::SmallAlloc(size_t size) {
  return NULL;
}

void *hoard::BigAlloc(size_t size, size_t alignment) {
  std::lock_guard<std::mutex> lock(bigAllocMutex);

  return NULL;
}

void hoard::SmallFree(void *ptr) {
}

void hoard::BigFree(void *ptr) {
  std::lock_guard<std::mutex> lock(bigAllocMutex);
}

void *hoard::InternalRealloc(void *ptr, size_t size) {
  return NULL;
}

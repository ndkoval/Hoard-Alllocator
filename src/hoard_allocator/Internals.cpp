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

	void *p = BigAlloc(size, alignment);
	//TODO
	return p;
}

void hoard::InternalFree(void *ptr) {
	BigFree(ptr);
}

void *hoard::SmallAlloc(size_t size) {
	return nullptr;
}

void *hoard::BigAlloc(size_t size, size_t alignment) {
	std::lock_guard<std::mutex> lock(bigAllocMutex);

	size_t total_size = round_up(size, alignment);

	void* ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == nullptr)
		return nullptr;

	bigAllocates.Add(ptr, total_size);

	return nullptr;
}

void hoard::SmallFree(void *ptr) {
}

void hoard::BigFree(void *ptr) {
	std::lock_guard<std::mutex> lock(bigAllocMutex);
	size_t size = bigAllocates.Get(ptr);
	munmap(ptr, size);
}

void *hoard::InternalRealloc(void *ptr, size_t size) {
	InternalFree(ptr);
	return InternalAlloc(size, 0);
}

#include <cstring>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <algorithm>

#include "utils.h"
#include "Internals.h"
#include "FreeSuperblockManager.h"
#include "GlobalHeap.h"
#include "LocalHeap.h"
#include "superblock.h"

namespace {
std::mutex big_alloc_mutex;

hoard::AllocFreeHashMap big_allocates_map;

hoard::FreeSuperblockManager globalFreeSuperblockManager;

hoard::GlobalHeap testGlobalHeap;

hoard::LocalHeap testLocalHeap;
}

namespace hoard {
void* InternalAlloc(size_t size, size_t alignment) {
	assert (IsValidAlignment(alignment));
	void *result;
	if (size >= kMaxBlockSize) {
		result = BigAlloc(size, alignment);
    } else {
		result = SmallAlloc(size, alignment);
	}
	return result;
}


void* SmallAlloc(size_t size, size_t alignment) {
	assert(!"small allocations needed");
	return nullptr;
}

void InternalFree(void *ptr) {
	if (!BigFree(ptr)) {
		SmallFree(ptr);
	}
}

void SmallFree(void *ptr) {
	assert(!"small allocations needed");
}

void* BigAlloc(size_t size, size_t alignment) {

	assert(IsValidAlignment(alignment));

	if(alignment > kPageSize) {

	}


	std::lock_guard<std::mutex> lock(big_alloc_mutex);

//	size_t total_size = RoundUp(size, alignment);
//
//	void *ptr = mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//	if (ptr == nullptr)
//		return nullptr;
//
//	trace("kakak");
//	big_allocates_map.Add(ptr, total_size);
//	big_allocates_map.PrintState();

	return nullptr;
}


//returns true if ptr was big allocation
bool BigFree(void *ptr) {
	std::lock_guard<std::mutex> lock(big_alloc_mutex);
	size_t size = big_allocates_map.Get(ptr);
	if(size == AllocFreeHashMap::NO_SUCH_KEY) {
		return false;
	}
	auto unmap_result = munmap(ptr, size);
	if (unmap_result == 0) {
		big_allocates_map.Remove(ptr);
		return true;
	} else {
		hoard::println("Big free unmap failed on adress: ", ptr);
		std::abort();
	}
}

void *InternalRealloc(void *ptr, size_t size) { // TODO optimize realloc before release
	InternalFree(ptr);
	return InternalAlloc(size);
}

}// hoard

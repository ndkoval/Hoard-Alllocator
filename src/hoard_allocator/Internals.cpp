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

namespace hoard {
namespace {

thread_local bool thread_inited = false;
std::atomic<bool> state_is_inited;
hoard::lock_t init_mutex;


struct HoardState {
	HoardState(): freeSuperblockManager(),
								test_global_heap(freeSuperblockManager, kMinBlockSize),
								test_local_heap(test_global_heap)
	{
		assert(!state_is_inited.load());
		//do something
		kRealPageSize_ = static_cast<size_t >(sysconf(_SC_PAGESIZE));
		kNumberOfCPU_ = static_cast<size_t>(sysconf(_SC_NPROCESSORS_ONLN));
		kNumberOfHeaps_ = kNumberOfCPU_ * 2;
		CheckPageSize();
	}

	hoard::lock_t big_alloc_mutex;
	hoard::AllocFreeHashMap big_allocates_map;
	hoard::FreeSuperblockManager freeSuperblockManager;
	hoard::GlobalHeap test_global_heap;
	hoard::LocalHeap test_local_heap;

private:
	size_t kRealPageSize_;
	size_t kNumberOfCPU_;
	size_t kNumberOfHeaps_;
	bool CheckPageSize() {
		hoard::trace("Page size is: ", kRealPageSize_);
		assert(hoard::kPageSize == kRealPageSize_ && "change kPageSize and recompile lib for your machine");
		return hoard::kPageSize == kRealPageSize_;
	}
};

char state_data[sizeof(HoardState)];
HoardState & state = *reinterpret_cast<HoardState *>(state_data); //ugly hack. State will initialized manually


void InitOnce() {
	if (!thread_inited) {
		if(!state_is_inited.load()) {
			hoard::lock_guard guard(init_mutex);
      if(!state_is_inited.load()) {
				new (&state) HoardState();
				state_is_inited.store(true); //after construct!
			}
		}
		thread_inited = true;
	}
}

} // namespace


void *InternalAlloc(size_t size, size_t alignment) {
	InitOnce();
	assert (IsValidAlignment(alignment));
	void *result;
	if (size >= kMaxBlockSize) {
		result = BigAlloc(size, alignment);
	} else {
		result = SmallAlloc(size, alignment);
	}
	return result;
}


void *SmallAlloc(size_t size, size_t alignment) {
	//TODO small allocations
	return BigAlloc(size, alignment);
}

void InternalFree(void *ptr) {
	InitOnce();
	if (ptr == nullptr)
		return;

	//big allocations always have page size alignment
	if (((size_t) ptr % kPageSize) || !BigFree(ptr)) {
		SmallFree(ptr);
	}
}

void SmallFree(void *ptr) {
//	SuperblockHeader *superblock = SuperblockHeader::Get(ptr);
//	assert(superblock->valid());


	//TODO small allocations
	BigFree(ptr);
}

void *BigAlloc(size_t size, size_t alignment) {
	assert(IsValidAlignment(alignment)); //TODO remove before release
	void *result_ptr = mmapAnonymous(size, alignment);
	if (!result_ptr) {
		return nullptr;
	}

	hoard::lock_guard guard(state.big_alloc_mutex);
	state.big_allocates_map.Add(result_ptr, size);
	return result_ptr;
}


//returns true if ptr was big allocation
bool BigFree(void *ptr) {
	hoard::lock_guard guard(state.big_alloc_mutex);
	size_t size = state.big_allocates_map.Get(ptr);
	if (size == AllocFreeHashMap::kNoSuchKey) {
		return false;
	}
	auto unmap_result = munmap(ptr, size);
	if (unmap_result == 0) {
		state.big_allocates_map.Remove(ptr);
		return true;
	} else {
		hoard::println("Big free unmap failed on adress: ", ptr);
		std::abort();
	}
}

void *InternalRealloc(void *ptr, size_t size) { // TODO optimize realloc before release
	if (ptr == nullptr) {
		return InternalAlloc(size);
	}

	if (size == 0) {
		InternalFree(ptr);
		return nullptr;
	}

	void *result = InternalAlloc(size);
	if (result == nullptr) {
		return nullptr;
	}
	size_t old_size;
	{
		lock_guard lock(state.big_alloc_mutex);
		old_size = state.big_allocates_map.Get(ptr);
	}
	memcpy(result, ptr, std::min(size, old_size));
	InternalFree(ptr);
	return result;
}

}// hoard

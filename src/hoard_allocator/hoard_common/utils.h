#ifndef UTILS_H
#define UTILS_H

#include "hoard_constants.h"
#include "tracing.h"
#include <assert.h>
#include <sys/mman.h>

namespace hoard {

constexpr inline bool IsPowerOf2(size_t n) {
	return ((n != 0) && !(n & (n - 1)));
}

constexpr inline bool IsValidAlignment(size_t alignment) {
	return (alignment % sizeof(void *)) != 0 ? false : IsPowerOf2(alignment);
}

constexpr inline size_t RoundUp(size_t what, size_t to) {
	return (what + to - 1) / to * to;
}

//constexpr unsigned int FloorLog2(unsigned int x) {
//  return sizeof(unsigned int) * 8 - 1 - __builtin_clz(x);
//}

constexpr unsigned long long FloorLog2(unsigned long long x) {
  return sizeof(unsigned long long) * 8 - 1 - __builtin_clzll(x);
}

inline void *GetFirstAlignedPointer(void *ptr, size_t alignment) {
	void *result = reinterpret_cast<void *> (RoundUp(reinterpret_cast<size_t> (ptr), alignment));
	check_debug(result >= ptr);
  check_debug(result < reinterpret_cast<void *>(reinterpret_cast<size_t> (ptr) + alignment));
	return result;
}


inline void *mmapAnonymous(void *addr, size_t size) {
	return mmap(addr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

inline void *mmapAnonymous(size_t size) {
	return mmapAnonymous(NULL, size);
}

inline void *mmapAnonymous(size_t size, size_t alignment) {
	assert(IsValidAlignment(alignment));
	//const size_t rounded_size = RoundUp(size, kPageSize);
  check_debug(size % kPageSize == 0); //callee should know mmaped size, or that will cause memory leaks
  auto const rounded_size = size;
	if (alignment <= kPageSize) {
		return mmapAnonymous(rounded_size);
	} else { //big alignment case
		const size_t mmaped_size = alignment + rounded_size - kPageSize;
		void * const mmaped_ptr = mmapAnonymous(mmaped_size);
		if (mmaped_ptr == MAP_FAILED) {
			return nullptr;
		}
		void * const result_ptr = GetFirstAlignedPointer(mmaped_ptr, alignment);
		const long long size_to_unmap_before = reinterpret_cast<size_t>(result_ptr) - reinterpret_cast<size_t>(mmaped_ptr);
		const  long long size_to_unmap_after = reinterpret_cast<size_t>(mmaped_size) - size_to_unmap_before - rounded_size;

    println(size_to_unmap_before, " ", size_to_unmap_after);
    print_object(size_to_unmap_after);
    check_debug(result_ptr >= mmaped_ptr, "result more or equal");
    check_debug(size_to_unmap_before >= 0);
    check_debug(size_to_unmap_after >= 0);
    check_debug(size_to_unmap_after + size_to_unmap_before + rounded_size == mmaped_size);
		if (size_to_unmap_before != 0) {
      check_fatal(munmap(mmaped_ptr, size_to_unmap_before) == 0, "Aligned mmap, unmap before failed");
    }
		if (size_to_unmap_after != 0) {
      check_fatal(munmap(reinterpret_cast<char*>(result_ptr) + rounded_size, size_to_unmap_after) == 0, "Aligned mmap, unmap after failed");
    }
		return result_ptr;
	}
}

}
#endif // UTILS_H

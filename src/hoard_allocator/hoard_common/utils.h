#ifndef UTILS_H
#define UTILS_H

#include "hoard_constants.h"
#include "tracing.h"
#include <assert.h>
#include <sys/mman.h>

namespace hoard {

constexpr bool IsPowerOf2(size_t n) {
	return ((n != 0) && !(n & (n - 1)));
}

constexpr bool IsValidAlignment(size_t alignment) {
	return (alignment % sizeof(void *)) != 0 ? false : IsPowerOf2(alignment);
}


inline size_t RoundUp(size_t what, size_t to) { // to is pow of two
  assert(IsPowerOf2(to));
	return (what + to - 1) & (~(to - 1));
}

//constexpr unsigned int FloorLog2(unsigned int x) {
//  return sizeof(unsigned int) * 8 - 1 - __builtin_clz(x);
//}

constexpr unsigned long long FloorLog2(unsigned long long x) {
  return sizeof(unsigned long long) * 8 - 1 - __builtin_clzll(x);
}

inline void *GetFirstAlignedPointer(void *ptr, size_t alignment) {
	void *result = reinterpret_cast<void *> (RoundUp(reinterpret_cast<size_t> (ptr), alignment));
	assert(result >= ptr);
  assert(result < reinterpret_cast<void *>(reinterpret_cast<size_t> (ptr) + alignment));
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
	const size_t rounded_size = RoundUp(size, kPageSize);
	if (alignment <= kPageSize) {
		return mmapAnonymous(rounded_size);
	} else { //big alignment case
		const size_t mmaped_size = alignment + rounded_size - kPageSize;
		void *mmaped_ptr = mmapAnonymous(mmaped_size);
		if (mmaped_ptr == MAP_FAILED) {
			return nullptr;
		}
		void *result_ptr = GetFirstAlignedPointer(mmaped_ptr, alignment);
		const size_t size_to_unmap_before = reinterpret_cast<size_t>(result_ptr) - reinterpret_cast<size_t>(mmaped_ptr);
		const size_t size_to_unmap_after = reinterpret_cast<size_t>(mmaped_size) - size_to_unmap_before - rounded_size;
		if (size_to_unmap_before != 0) {
      if (munmap(mmaped_ptr, size_to_unmap_before) != 0) {
        fatal_error("Aligned mmap, unmap before failed");
      }
		}
		if (size_to_unmap_after != 0) {
      if (munmap(reinterpret_cast<char*>(result_ptr) + rounded_size, size_to_unmap_after) != 0) {
        fatal_error("Aligned mmap, unmap before failed");
      }
		}
		return result_ptr;
	}
}

}
#endif // UTILS_H

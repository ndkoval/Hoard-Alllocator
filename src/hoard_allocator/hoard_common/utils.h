#ifndef UTILS_H
#define UTILS_H

#include "hoard_constants.h"
#include <assert.h>
#include<sys/mman.h>

namespace hoard {

constexpr bool IsPowerOf2(size_t n) {
  return ((n != 0) && !(n & (n - 1)));
}

constexpr bool IsValidAlignment(size_t alignment) {
  return (alignment % sizeof(void *)) != 0 ? false : IsPowerOf2(alignment);
}

constexpr size_t RoundUp(size_t what, size_t to) {
  return (what + to - 1) / to * to;
}

inline void * GetFirstAllignedPointer(void *ptr, size_t alignment) {
  return reinterpret_cast<void *> (RoundUp(reinterpret_cast<size_t> (ptr), alignment));
}


inline void *mmapAnonymous(void *addr, size_t size) {
  assert(size % kPageSize == 0);
  return mmap(addr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

inline void *mmapAnonymous(size_t size) {
  return mmapAnonymous(NULL, size);
}

inline void* mmapAligned(size_t size, size_t alignment) {
  //TODO make tests
  assert(IsValidAlignment(alignment));
  const size_t rounded_size = RoundUp(size, kPageSize);
  if (alignment <= kPageSize) {
    return mmapAnonymous(rounded_size);
  } else { //big allignment case
    const size_t mmaped_size = alignment + rounded_size - kPageSize;
    void *mmaped_ptr = mmapAnonymous(mmaped_size);
    if(mmaped_ptr == MAP_FAILED) {
      return nullptr;
    }
    void *result_ptr = GetFirstAllignedPointer(mmaped_ptr, alignment);
    const size_t size_to_unmap_before = reinterpret_cast<size_t>(mmaped_ptr) - reinterpret_cast<size_t>(result_ptr);
    const size_t size_to_unmap_after = reinterpret_cast<size_t>(mmaped_size) - size_to_unmap_before - rounded_size;
    if (size_to_unmap_before) {
      assert(munmap(mmaped_ptr, size_to_unmap_before) != -1);
    }
    if (size_to_unmap_after) {
      assert(munmap(reinterpret_cast<char *>(result_ptr) + rounded_size, size_to_unmap_after) != -1);
    }
    return result_ptr;
  }

}

}


#endif // UTILS_H

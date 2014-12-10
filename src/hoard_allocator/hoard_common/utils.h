#ifndef UTILS_H
#define UTILS_H

#include "hoard_constants.h"
#include <assert.h>
#include <sys/mman.h>

namespace hoard {

constexpr bool isPowerOf2(size_t n) {
  return ((n != 0) && !(n & (n - 1)));
}

constexpr bool IsValidAlignment(size_t alignment) {
  return (alignment % sizeof(void *)) != 0 ? false :  isPowerOf2(alignment);
}

inline char* get_first_aligned_pointer(char *ptr, size_t alignment) {
  size_t offset = ((size_t) ptr) % alignment;
  return ptr + offset;
}

constexpr size_t round_up(size_t what, size_t to) {
  return (what + to - 1) / to * to;
}

inline void *mmapAnonymous(void *addr, size_t length) {
  return mmap(addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
}

inline void *mmapAnonymous(size_t length) {
  return mmapAnonymous(NULL, length);
}

}


#endif // UTILS_H

#include <hoard_constants.h>
#include "tracing.h"

extern "C"
void *malloc(size_t size) __THROW {
  hoard::trace("malloc: ", size);
  return nullptr;
}

extern "C"
void *calloc(size_t n, size_t size) __THROW {
  hoard::trace("calloc: n=", n, "; size=", size);
  return nullptr;
}

extern "C"
void free(void *ptr) __THROW {
  hoard::trace("free: ", ptr);
}

extern "C"
void *realloc(void *ptr, size_t size) __THROW {
  hoard::trace("realloc: ptr=", ptr, "; size=", size);
  return nullptr;
}

extern "C"
int posix_memalign(void **memptr, size_t alignment, size_t size) __THROW {
  hoard::trace("posix_memalign");
  std::abort();
}

extern "C"
void *valloc(size_t size) __THROW {
  hoard::trace("valloc: ", size);
  return nullptr;
}

extern "C"
void *memalign(size_t boundary, size_t size) __THROW {
  hoard::trace("memalign: ", boundary, ' ', size);
  return nullptr;
}
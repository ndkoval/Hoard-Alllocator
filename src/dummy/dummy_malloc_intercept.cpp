#include <cstdlib>

#include "tracing.h"

extern "C"
void *malloc(size_t size) {
    hoard::trace("malloc: ", size);
    std::abort();
}

extern "C"
void *calloc(size_t n, size_t size) {
    hoard::trace("calloc: n=", n, "; size=", size);
    std::abort();
}

extern "C"
void free(void *ptr) {
    hoard::trace("free: ", ptr);
    std::abort();
}

extern "C"
void *realloc(void *ptr, size_t size) {
    hoard::trace("realloc: ptr=", ptr, "; size=", size);
    std::abort();
}

extern "C"
int posix_memalign(void **memptr, size_t alignment, size_t size) {
    std::abort();
}

extern "C"
void *valloc(size_t size) {
    std::abort();
}

extern "C"
void *memalign(size_t boundary, size_t size) {
    std::abort();
}

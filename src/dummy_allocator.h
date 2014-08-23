#include <cerrno>

#include "internals.h"
#include "tracing.h"

extern "C"
void* malloc(size_t size)
{
    exit(-1);
}

extern "C"
void* calloc(size_t n, size_t size)
{
    exit(-1);
}

extern "C"
void free(void *ptr)
{
    exit(-1);
}

extern "C"
void* realloc(void *ptr, size_t size)
{
    exit(-1);
}

extern "C"
int posix_memalign(void** memptr, size_t alignment, size_t size)
{
    exit(-1);
}

extern "C"
void *valloc(size_t size)
{
    exit(-1);
}

extern "C"
void *memalign(size_t boundary, size_t size)
{
    exit(-1);
}

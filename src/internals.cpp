#include "internals.h"
#include "tracing.h"

#include <cstring>

#include <unistd.h>
#include <sys/mman.h>

#include <algorithm>
#include "utils.h"


void* hoard::internal_alloc(size_t size)
{
    return NULL;
}

void* hoard::internal_alloc(size_t size, size_t alignment)
{
    return NULL;
}

void hoard::internal_free(void* ptr)
{
    if (ptr == NULL)
        return;
}

void* hoard::small_alloc(size_t size)
{
    return NULL;
}

void* hoard::big_alloc(size_t size, size_t alignment)
{

}

void hoard::small_free(void* ptr)
{

}

void hoard::big_free(void* ptr)
{

}

void* hoard::internal_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return internal_alloc(size);

    void* new_data = internal_alloc(size);
    if (new_data == NULL)
        return NULL;

    //should be changed with allocated size
    size_t old_size = size;
    memcpy(new_data, ptr, size);

    internal_free(ptr);

    return new_data;
}

bool hoard::is_valid_alignment(size_t alignment)
{
    if ((alignment % sizeof(void*)) != 0)
        return false;

    if (!is_power_of_2(alignment))
        return false;

    return true;
}

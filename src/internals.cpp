#include "internals.h"
#include "tracing.h"

#include <cstring>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unistd.h>
#include <sys/mman.h>
#include <algorithm>

#include "utils.h"
#include "mmap_std_allocator.h"
#include "internals.h"
#include "tracing.h"

namespace
{
    bool is_power_of_2(size_t n)
    {
        return ((n != 0) && !(n & (n - 1))); 
    }

    std::mutex big_alloc_mutex;
    std::unordered_map<void*, size_t,
        std::hash<void*>,
        std::equal_to<void*>,
        std::allocator<std::pair<void*, size_t> > > big_allocates;
}

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
    return mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void* hoard::big_alloc(size_t size, size_t alignment)
{
    void* p = mmap(NULL, size + alignment, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void hoard::small_free(void* ptr)
{
    munmap(ptr, 4096);
}

void hoard::big_free(void* ptr)
{
    auto it = big_allocates.find(ptr);
    size_t size = (*it).second;
    big_allocates.erase(it);
    munmap(ptr, size);
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

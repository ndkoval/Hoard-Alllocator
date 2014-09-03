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
//#include "free_superblock_manager.h" // build errors, i'm working on it. Skipor
//#include "global_heap.h"
//#include "local_heap.h"
//#include "superblock.h"

namespace
{
    std::mutex big_alloc_mutex;
    std::unordered_map<void*, size_t,
        std::hash<void*>,
        std::equal_to<void*>,
        hoard::mmap_std_allocator<std::pair<void*, size_t> > > big_allocates;
   // hoard::free_superblock_manager(); // build errors, i'm working on it. Skipor
   // hoard::global_heap();
    //hoard::local_heap();
}


void* hoard::internal_alloc(size_t size)
{
    big_alloc(size, 8);
}

void* hoard::internal_alloc(size_t size, size_t alignment)
{
    big_alloc(size, alignment);
}

void hoard::internal_free(void* ptr)
{
    big_free(ptr);
}

void* hoard::small_alloc(size_t size)
{
    return mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void* hoard::big_alloc(size_t size, size_t alignment)
{
    std::lock_guard<std::mutex> lock(big_alloc_mutex);

    char* p = (char*) mmap(NULL, size + alignment, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    char* ptr = (char*) hoard::get_first_aligned_pointer(p, alignment);

    big_allocates.emplace(p, size + alignment);

    return ptr;
}

void hoard::small_free(void* ptr)
{
    munmap(ptr, 4096);
}

void hoard::big_free(void* ptr)
{
    std::lock_guard<std::mutex> lock(big_alloc_mutex);

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

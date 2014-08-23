#include <vector>

#include "gtest/gtest.h"
#include "mmap_allocator.h"

TEST(mmap_allocator, alloc)
{
    hoard::mmap_allocator<int> a;
    std::vector<int> v(a);
    for (int i = 0; i < 1000; i++) {
        v.push_back(i);
    }
}

TEST(mmap_allocator, free)
{
    hoard::mmap_allocator<int> a;
    std::vector<int> v(a);
    v.~vector();
}

#include <vector>
#include <assert.h>

#include "mmap_std_allocator.h"

int main()
{
    std::vector<int, hoard::mmap_std_allocator<int>> v;
    for (int i = 0; i < 1000; i++) {
        v.push_back(i);
    }

    for (int i = 0; i < 1000; i++) {
        assert(v[i] == i);
    }

    return 0;
}

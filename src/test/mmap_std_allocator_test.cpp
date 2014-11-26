#include <vector>

#include "gtest/gtest.h"
#include "mmap_std_allocator.h"

TEST(mmap_std_allocator, alloc) {
  std::vector<int, hoard::mmap_std_allocator<int>> v;
  for (int i = 0; i < 1000; i++) {
    v.push_back(i);
  }

  for (int i = 0; i < 1000; i++) {
    ASSERT_EQ(v[i], i);
  }
}

TEST(mmap_std_allocator, free) {
  std::vector<int, hoard::mmap_std_allocator<int>> v;
  v.~vector();
}

TEST(mmap_std_allocator, complex_structures_vector) {
  std::vector<std::vector<int>, hoard::mmap_std_allocator < std::vector<int> > > v;

  for (int i = 0; i < 1000; i++) {
    v.push_back(std::vector<int>());
    v[i].push_back(i);
  }

  for (int i = 0; i < 1000; i++) {
    ASSERT_EQ(v[i][0], i);
  }
}

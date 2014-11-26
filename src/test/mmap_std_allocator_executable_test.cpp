#include <vector>
#include <list>
#include <assert.h>

#include "mmap_std_allocator.h"

void test_vector() {
  std::vector<int, hoard::mmap_std_allocator<int>> v;
  for (int i = 0; i < 1000; i++) {
    v.push_back(i);
  }

  for (int i = 0; i < 1000; i++) {
    assert(v[i] == i);
  }
}

void test_list() {
  typedef std::vector<int, hoard::mmap_std_allocator<int>> int_vector_with_mmap_allocator;
  std::list<int_vector_with_mmap_allocator, hoard::mmap_std_allocator < int_vector_with_mmap_allocator>>
  l;

  for (int i = 0; i < 1000; i++) {
    l.push_back(int_vector_with_mmap_allocator(10));
  }
}

int main() {
  test_vector();
  test_list();

  return 0;
}

//
// Created by  Vladimir Skipor on 05/04/15.
//

#ifndef HOARD_ALLOCATOR_ALLOCATION_TESTER_H
#define HOARD_ALLOCATOR_ALLOCATION_TESTER_H

#include <unordered_map>
#include <cstring>
#include "rand.h"

template <size_t kBlockSize>
class AllocationTester {
public:
  using KeyType = void *;
  using ValueType = std::array<char, kBlockSize>;


  void Add(KeyType ptr) {
    std::memcpy(allocation_map_[ptr].data(), ptr, kBlockSize);
  }

  bool ReadTest(KeyType ptr) {
    return std::memcmp(allocation_map_[ptr].data(), ptr, kBlockSize) == 0;
  }

  bool WriteTest(KeyType ptr, void * data) {
    bool res = true;
    if(allocation_map_.count(ptr)) {
      res = ReadTest(ptr);
    }
    std::memcpy(ptr, data, kBlockSize);
    std::memcpy(allocation_map_[ptr].data(), data, kBlockSize);
    return res;
  }

  bool WriteRandomTest(KeyType ptr) {
    return WriteTest(ptr, GetRandomValue().data());
  }

  bool TestReadAll() {
    bool res = true;
    for(auto kv_pair : allocation_map_) {
      res &= ReadTest(kv_pair.first, kv_pair.second);
    }
    return res;
  }

  bool TestWriteAll() {
    bool res = true;
    for(auto kv_pair : allocation_map_) {
      res &= WriteRandomTest(kv_pair.first);
    }
    return res;
  }







private:

  bool ReadTest(KeyType ptr, ValueType & value) {
    return std::memcmp(value.data(), ptr, kBlockSize) == 0;
  }
  ValueType & GetRandomValue() {
    static ValueType random_data;
    std::generate(random_data.begin(), random_data.end(), [&]() -> char {
      return rand_.next24();
    });
    return random_data;
  }


  std::unordered_map<KeyType, ValueType> allocation_map_;
  Rand rand_;

};

#endif //HOARD_ALLOCATOR_ALLOCATION_TESTER_H

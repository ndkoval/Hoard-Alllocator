//
// Created by  Vladimir Skipor on 04/04/15.
//

#include <gtest/gtest.h>
#include <algorithm>

#include <cstring>
#include <allocation_tester.h>

#include <Internals.h>
using namespace hoard;

namespace {
  constexpr size_t kBlockSize = 257;
//  size_t kBlockSize = kMaxBlockSize / 4;
  constexpr size_t kIterationNum = 10000;


}

class InternalsTest : public ::testing::Test {
public:


protected:
  InternalsTest() {
    ResetState();
  }

  virtual void TearDown() override { }
  virtual void SetUp() override { };
};


TEST_F(InternalsTest, SmallAllocTest) {
  AllocationTester<kBlockSize> allocation_tester;
  auto allocations = std::vector<void *>();
  for(size_t i = 0; i < kIterationNum; ++i) {
    allocations.push_back(SmallAlloc(kBlockSize, kBlockSize));
    allocation_tester.WriteRandomTest(allocations.back());
  }

  allocation_tester.TestWriteAll();

}

TEST_F(InternalsTest, SmallFreeTest) {
  AllocationTester<kBlockSize> allocation_tester;
  auto allocations = std::vector<void *>();
  for(size_t i = 0; i < kIterationNum; ++i) {
    allocations.push_back(InternalAlloc(kBlockSize));
    allocation_tester.WriteRandomTest(allocations.back());
  }

  allocation_tester.TestWriteAll();

  for(void * ptr : allocations) {
    allocation_tester.Remove(ptr);
    InternalFree(ptr);
  }

  allocation_tester.TestWriteAll();

  for(size_t i = 0; i < kIterationNum; ++i) {
    allocations.push_back(InternalAlloc(kBlockSize));
  }
  allocation_tester.TestWriteAll();

}

TEST_F(InternalsTest, SmallDifferentHeapsFreeTest) {
  AllocationTester<kBlockSize> allocation_tester1;
  AllocationTester<kBlockSize*4> allocation_tester2;
  auto allocations = std::vector<void *>();
  for(size_t i = 0; i < kIterationNum; ++i) {
    allocations.push_back(InternalAlloc(kBlockSize));
    allocation_tester1.WriteRandomTest(allocations.back());
  }

  allocation_tester1.TestWriteAll();

  for(void * ptr : allocations) {
    allocation_tester1.Remove(ptr);
    InternalFree(ptr);
  }

  allocation_tester1.TestWriteAll();
  allocations.resize(0);

  for(size_t i = 0; i < kIterationNum; ++i) {
    allocations.push_back(InternalAlloc(kBlockSize*4));
    allocation_tester2.WriteRandomTest(allocations.back());
  }

  allocation_tester2.TestWriteAll();
  for(void * ptr : allocations) {
    allocation_tester2.Remove(ptr);
    InternalFree(ptr);
  }

}


TEST_F(InternalsTest, BigAlloc) {

  constexpr size_t kBigAllocSize = kMaxBlockSize * 4;
  AllocationTester<kBigAllocSize> allocation_tester;


  auto allocations = std::vector<void *>();
  for(size_t i = 0; i < kIterationNum /100; ++i) {
    allocations.push_back(InternalAlloc(kBigAllocSize));
    allocation_tester.WriteRandomTest(allocations.back());
  }

  allocation_tester.TestWriteAll();

  for(void * ptr : allocations) {
    InternalFree(ptr);
    allocation_tester.Remove(ptr);
  }


  for(size_t i = 0; i < kIterationNum; ++i) {
    allocations.push_back(InternalAlloc(kBigAllocSize));
  }
  allocation_tester.TestWriteAll();

}


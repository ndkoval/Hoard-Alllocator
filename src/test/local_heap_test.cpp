#include <gtest/gtest.h>
#include <algorithm>

#define private public
#include <LocalHeap.h>
#undef private
#include <cstring>
#include <allocation_tester.h>

using namespace hoard;

namespace {
constexpr size_t kBlockSize = 16;
//  size_t kBlockSize = kMaxBlockSize / 4;


void AddSuperblockToHeap(LocalHeap & heap, Superblock * superblock) {
  superblock->header().set_owner(&heap);
  heap.AcceptSuperblock(superblock);
}
  void AddSuperblockToHeap(GlobalHeap & heap, Superblock * superblock) {
    heap.AddSuperblock(superblock);
  }
}

class LocalHeapTest : public ::testing::Test {
public:
  FreeSuperblockManager manager;
  GlobalHeap global_heap;
  LocalHeap local_heap;
  LocalHeap another_local_heap;

protected:
  LocalHeapTest()
      : Test(),
        manager(),
        global_heap(manager, kBlockSize),
        local_heap(global_heap),
        another_local_heap(global_heap) {
  }

  virtual void TearDown() override {
  } virtual void SetUp() override { }; }; 
TEST(BinNumTest, BellowAboveDifferentBins) {
  const int total_blocks = 1024 * 1024;
  const int max_bellow = total_blocks / kEmptynessFactor;
  const int min_above = max_bellow + 1;
  const int kBellowThresholdSuperblocksBinNum = LocalHeap::kBellowThresholdSuperblocksBinNum;
  EXPECT_EQ(LocalHeap::GetBinNum(max_bellow, total_blocks), kBellowThresholdSuperblocksBinNum);
  EXPECT_TRUE (
      LocalHeap::GetBinNum(min_above, total_blocks)
          > LocalHeap::GetBinNum(max_bellow, total_blocks)
  );
}

TEST(BinNumTest, EmptyNonEmptyDifferentBins) {
  const int total_blocks = 1024 * 1024;
  EXPECT_EQ(LocalHeap::GetBinNum(0, total_blocks), 0);
  EXPECT_EQ(LocalHeap::GetBinNum(1, total_blocks), 1);
}

TEST_F(LocalHeapTest, FullNonFullDifferentBins) {
  const int total_blocks = 1024 * 1024;
  const size_t kAlmostFull = LocalHeap::kAlmostFullBlocksBinNum;
  EXPECT_EQ(LocalHeap::GetBinNum(total_blocks - 1, total_blocks), kAlmostFull);
  EXPECT_EQ(LocalHeap::GetBinNum(total_blocks, total_blocks), kAlmostFull + 1);
}

TEST(BinNumTest, GetBinNumMonotony) {
  const int total_blocks = 1024 * 1024;
  for (int i_current_blocs = 1; i_current_blocs <= total_blocks; ++i_current_blocs) {
    EXPECT_TRUE(LocalHeap::GetBinNum(i_current_blocs - 1, total_blocks) <= LocalHeap::GetBinNum(i_current_blocs, total_blocks));
  }
}

TEST_F(LocalHeapTest, HeapGetSuperblockInvariantTest) {
  auto stop_trace = StopTraceGuard();
  Superblock * superblock_example = Superblock::Make(kBlockSize);
  for (size_t i = 0; i <= kSuperblocksInLocalHeapLowBound * 2; ++i) {
    //local_heap.GetSuperblock();
    for (size_t j = 0; j < superblock_example->header().size(); ++j) {
      local_heap.Alloc();
    }
    EXPECT_EQ(local_heap.superblock_count(), i + 1);
    EXPECT_EQ(local_heap.size(), superblock_example->header().size() * (i + 1));
    EXPECT_EQ(local_heap.blocks_allocated(), superblock_example->header().size() * (i + 1));
  }
  for (SuperblockStack & bin : local_heap.bins_) {
  if(!bin.IsEmpty()) {
      Superblock* current = bin.Top();
      while (current != nullptr) {
        EXPECT_EQ(current->header().owner(), &local_heap);
        current = current->header().next();
      }
    }
  }
}

TEST_F(LocalHeapTest, TransferToParentTest) {
  void* ptr = local_heap.Alloc();
  EXPECT_EQ(local_heap.blocks_allocated_, 1);
  SuperblockHeader *header = SuperblockHeader::Get(ptr);
  Superblock *superblock = header->GetSuperblock();
  EXPECT_TRUE(header->valid());
  local_heap.Free(superblock, ptr);
  EXPECT_EQ(local_heap.blocks_allocated_, 0);
}

TEST_F(LocalHeapTest, HeapGetSuperblockThresholdTest) {
  for (size_t i = 0; i < kSuperblocksInLocalHeapLowBound; ++i) {
    local_heap.GetSuperblock();
    EXPECT_FALSE(local_heap.HeapBellowEmptynessThreshold());
  }
  for (size_t i = 0; i < kSuperblocksInLocalHeapLowBound; ++i) {
    local_heap.GetSuperblock();
    EXPECT_TRUE(local_heap.HeapBellowEmptynessThreshold());
  }
}

TEST_F(LocalHeapTest, TestMoveToNextBin) {
  SuperblockHeader *header = SuperblockHeader::Get(local_heap.Alloc());
  for (size_t i = 1; i < header->size(); i++) {
    size_t bin_num = local_heap.GetBinNum(i, header->size());
    ASSERT_FALSE(local_heap.bins_[bin_num].IsEmpty());
    local_heap.Alloc();
  }
}


TEST_F(LocalHeapTest, TestBinStateInvalidationOnFree) {
  void *ptr = local_heap.Alloc();
  ASSERT_FALSE(local_heap.bins_[1].IsEmpty());
  auto * superblock = Superblock::Get(ptr);
  local_heap.Free(superblock, ptr);
  ASSERT_FALSE(local_heap.bins_[0].IsEmpty());
}

TEST_F(LocalHeapTest, TestAllBinVisitedAlloc) {
  auto * superblock = Superblock::Make(kBlockSize);
  auto& header = superblock->header();

  AddSuperblockToHeap(local_heap, superblock);

  auto SuperblockIsInCorrectBin = [&]() {
    return local_heap.bins_[LocalHeap::GetBinNum(header)].Contains(superblock);

  };

  auto visited_bins = std::vector<bool>(LocalHeap::kBinCount, false);
  size_t prev_bin_num = 0;
  visited_bins[prev_bin_num] = true;
  ASSERT_TRUE(SuperblockIsInCorrectBin());
  while (!header.full()) {
    local_heap.Alloc();
    ASSERT_TRUE(SuperblockIsInCorrectBin());
    const auto current_bin = LocalHeap::GetBinNum(header);
    ASSERT_GE(current_bin, prev_bin_num);
    prev_bin_num = current_bin;
    visited_bins.at(current_bin) = true;
    local_heap.CheckInvariantsOrDie();
  }

  ASSERT_EQ(0, std::count(visited_bins.begin(), visited_bins.end(), false));
}

TEST_F(LocalHeapTest, TestAllBinVisitedFree) {
  auto * superblock = Superblock::Make(kBlockSize);
  auto& header = superblock->header();

  AddSuperblockToHeap(local_heap, superblock);

  auto SuperblockIsInCorrectBin = [&]() {
    return local_heap.bins_[LocalHeap::GetBinNum(header)].Contains(superblock);
  };
  auto allocated = std::vector<void*>();
  while (!header.full()) {
    allocated.push_back(local_heap.Alloc());
  }
  local_heap.CheckInvariantsOrDie();

  auto visited_bins = std::vector<bool>(LocalHeap::kBinCount, false);
  ASSERT_TRUE(SuperblockIsInCorrectBin());
  size_t prev_bin_num = LocalHeap::GetBinNum(header);
  visited_bins[prev_bin_num] = true;
  std::random_shuffle(allocated.begin(), allocated.end());

  for (auto ptr : allocated) {
    local_heap.Free(superblock, ptr);
    ASSERT_TRUE(SuperblockIsInCorrectBin());
    const auto current_bin = LocalHeap::GetBinNum(header);
    ASSERT_LE(current_bin, prev_bin_num);
    prev_bin_num = current_bin;
    visited_bins.at(current_bin) = true;
    local_heap.CheckInvariantsOrDie();
  }

  ASSERT_EQ(0, std::count(visited_bins.begin(), visited_bins.end(), false));
}


TEST_F(LocalHeapTest, MoveToFrontTest) {
  auto stop_trace = StopTraceGuard();
  auto superblocks = std::vector<Superblock *> (8);
  std::generate(superblocks.begin(), superblocks.end(), []() {
    return  Superblock::Make(kBlockSize);
  });
  auto* const some_superblock = superblocks[superblocks.size()/ 2];
  void* const ptr = some_superblock->header().Alloc();

  for (auto* superblock :superblocks) {
    while (superblock->header().blocks_allocated() < superblock->header().size() - 1) {
      superblock->header().Alloc();
    }
    superblock->header().set_owner(&local_heap);
    local_heap.AcceptSuperblock(superblock);
    local_heap.CheckInvariantsOrDie();
  }

  local_heap.Free(some_superblock, ptr);
}


TEST_F(LocalHeapTest, TestStressAlloc) {
  auto stop_trace = StopTraceGuard();
  SuperblockHeader *header = SuperblockHeader::Get(local_heap.Alloc());
  size_t blocksInSuperblock = header->size();
  for (size_t i = 1; i < blocksInSuperblock * 10; i++) {
    local_heap.Alloc();
  }
}

TEST_F(LocalHeapTest, TestStressAllocFree) {
//  auto stop_trace = StopTraceGuard();

  ASSERT_EQ(local_heap.blocks_allocated(), 0);

  SuperblockHeader *header = SuperblockHeader::Get(local_heap.Alloc());
  const size_t blocksInSuperblock = header->size();

  AllocationTester<kBlockSize> allocation_tester;

  auto allocated = std::vector<void *>();
  for (size_t i = 0; i < blocksInSuperblock * 20; i++) {
    allocated.push_back(local_heap.Alloc());
    local_heap.CheckInvariantsOrDie();
    ASSERT_TRUE(allocation_tester.WriteRandomTest(allocated.back()));
  }

  std::random_shuffle(allocated.begin(), allocated.end());
  ASSERT_TRUE(allocation_tester.TestWriteAll());
//
  auto allocations_moved_to_parent = std::vector<void *>();
  for (void *ptr : allocated) {
    auto *superblock = Superblock::Get(ptr);
    superblock->header().owner()->Free(superblock, ptr);
//    if (superblock->header().owner() == &local_heap) {
//      local_heap.Free(superblock, ptr);
//    } else {
//      allocations_moved_to_parent.push_back(ptr);
//    }
    local_heap.CheckInvariantsOrDie();
  }

}

TEST_F(LocalHeapTest, TestStressTwoHeapsAllocFree) {
  auto stop_trace = StopTraceGuard();

  ASSERT_EQ(local_heap.blocks_allocated(), 0);

  SuperblockHeader *header = SuperblockHeader::Get(local_heap.Alloc());
  const size_t blocksInSuperblock = header->size();


  AllocationTester<kBlockSize> allocation_tester;
  auto allocated = std::vector<void *>();
  for (size_t i = 0; i < blocksInSuperblock * 5; i++) {
    allocated.push_back(local_heap.Alloc());
    local_heap.CheckInvariantsOrDie();
    allocation_tester.WriteRandomTest(allocated.back());
  }
  for (size_t i = 0; i < blocksInSuperblock * 5; i++) {
    allocated.push_back(another_local_heap.Alloc());
    local_heap.CheckInvariantsOrDie();
    allocation_tester.WriteRandomTest(allocated.back());
  }

  ASSERT_TRUE(allocation_tester.TestWriteAll());

  std::random_shuffle(allocated.begin(), allocated.end());
//
  auto allocations_moved_to_parent = std::vector<void *>();
  for (void *ptr : allocated) {
    if(std::rand() % 100 < 80){

    auto *superblock = Superblock::Get(ptr);
//    superblock->header().owner()->Free(superblock, ptr);
    if (superblock->header().owner() == &local_heap) {
      local_heap.Free(superblock, ptr);
    } else {
      allocations_moved_to_parent.push_back(ptr);
    }
    local_heap.CheckInvariantsOrDie();
    }
  }

  for(void * ptr : allocations_moved_to_parent) {
    ASSERT_TRUE(allocation_tester.WriteRandomTest(ptr));
  }


  allocated.resize(0);
  for (size_t i = 0; i < blocksInSuperblock * 5; i++) {
    allocated.push_back(local_heap.Alloc());
    local_heap.CheckInvariantsOrDie();
    allocation_tester.WriteRandomTest(allocated.back());
  }
  for (size_t i = 0; i < blocksInSuperblock * 5; i++) {
    allocated.push_back(another_local_heap.Alloc());
    local_heap.CheckInvariantsOrDie();
    allocation_tester.WriteRandomTest(allocated.back());
  }
  ASSERT_TRUE(allocation_tester.TestWriteAll());

  std::random_shuffle(allocated.begin(), allocated.end());

  for (void *ptr : allocated) {
    auto *superblock = Superblock::Get(ptr);
    superblock->header().owner()->Free(superblock, ptr);
    local_heap.CheckInvariantsOrDie();
  }

}

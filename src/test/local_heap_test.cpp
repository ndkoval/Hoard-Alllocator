#include <gtest/gtest.h>

#define private public
#include <LocalHeap.h>
#undef private

using namespace hoard;

namespace {
size_t kBlockSize = 16;
}

class LocalHeapTest : public ::testing::Test {
public:
  FreeSuperblockManager manager;
  GlobalHeap global_heap;
  LocalHeap local_heap;
protected:
  LocalHeapTest()
      : Test(),
        manager(),
        global_heap(manager, kBlockSize),
        local_heap(global_heap) {
  }

  virtual void TearDown() override {
  }

  virtual void SetUp() override {
  };

};


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
  Superblock * superblock_example = Superblock::Make();
  superblock_example->header().Init(kBlockSize);
  for (size_t i = 0; i <= kSuperblocksInLocalHeapLowBound * 2; ++i) {
    local_heap.GetSuperblock();
    EXPECT_EQ(local_heap.superblock_count(), i + 1);
    EXPECT_EQ(local_heap.size(), superblock_example->header().size() * (i + 1));
    EXPECT_EQ(local_heap.blocks_allocated(), 0);
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

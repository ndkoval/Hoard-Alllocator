#include <gtest/gtest.h>
#include <GlobalHeap.h>
#include <superblock_load_test.h>

#define private public

#include "HoardState.h"

#undef private

using namespace hoard;

namespace {
size_t kBlockSize = 16;
}

class GlobalHeapTest : public ::testing::Test {
public:
  FreeSuperblockManager manager;
  GlobalHeap global_heap;
protected:
  GlobalHeapTest() : Test(), manager(), global_heap(manager, kBlockSize) {
  }

  virtual void TearDown() override {
  }

  virtual void SetUp() override {
  };

};

struct GlobalHeapStateTest : public ::testing::Test {
  HoardState state;
  GlobalHeap &global_heap1;
  GlobalHeap &global_heap2;

  GlobalHeapStateTest()
      : global_heap1(state.global_heap_group_[HoardState::GetHeapIndexByBlockSize(kBlockSize)]),
        global_heap2(state.global_heap_group_[HoardState::kDifferntBlockSizes - 1]) {
  }
};


TEST_F(GlobalHeapTest, get_superblock_from_empty) {
	Superblock *superblock = global_heap.GetSuperblock();
	ASSERT_NE(superblock, nullptr);
  SuperblockLoadTest(*superblock);
}

TEST_F(GlobalHeapTest, owner_is_heap_after_get) {
	Superblock *superblock = global_heap.GetSuperblock();
	ASSERT_EQ(superblock->header().owner(), &global_heap);
}

TEST_F(GlobalHeapTest, owner_changed_on_unempty_superblock_after_add) {
	Superblock * superblock = Superblock::Make();
	superblock->header().Init(kBlockSize);
	superblock->header().Alloc();
	global_heap.AddSuperblock(superblock);
	ASSERT_EQ(superblock->header().owner(), &global_heap);
}

TEST_F(GlobalHeapStateTest, SuperblockTransferTest) {
  Superblock * superblock = global_heap1.GetSuperblock();
  SuperblockLoadTest(*superblock);

  void *const ptr = superblock->header().Alloc();
  ASSERT_FALSE(superblock->header().empty());
  global_heap1.AddSuperblock(superblock);
  EXPECT_EQ(superblock->header().owner(), &global_heap1);

  global_heap1.Free(superblock, ptr);
  println("After OnFree");
  EXPECT_EQ(superblock->header().owner(), nullptr);
//
  Superblock *transfered_superblock = global_heap2.GetSuperblock();
  EXPECT_EQ(superblock, transfered_superblock);
  SuperblockLoadTest(*superblock);

}

TEST_F(GlobalHeapStateTest, RandomFreeFromChain) {
  auto allocated = std::vector<void*>();
  for(size_t iteration = 0; iteration < 100; ++iteration) {
    auto* superblock = Superblock::Make(kBlockSize);
    allocated.push_back(superblock->header().Alloc());
    global_heap1.AddSuperblock(superblock);
    global_heap1.CheckInvariantsOrDie();
  }
  std::random_shuffle(allocated.begin(), allocated.end());

  for (void* ptr : allocated) {
    Superblock* superblock = Superblock::Get(ptr);
    global_heap1.Free(superblock, ptr);
    global_heap1.CheckInvariantsOrDie();
    ASSERT_EQ(state.superblock_manager_.GetSuperblock(), superblock);
  }

}



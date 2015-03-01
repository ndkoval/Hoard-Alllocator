#include <gtest/gtest.h>
#include <GlobalHeap.h>

#define private public

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

TEST_F(GlobalHeapTest, get_superblock_from_empty) {
	Superblock *superblock = global_heap.GetSuperblock();
	ASSERT_NE(superblock, nullptr);
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
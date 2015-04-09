#include <gtest/gtest.h>

//#include "Superblock.h"
#define private public

#include "FreeSuperblockManager.h"

#include "HoardState.h"

#undef private

using namespace hoard;


#ifdef HOARD_USE_SUPERBLOCK_MANAGER
struct StateFreeSuperblockManagerTest : public ::testing::Test {
  HoardState state;
  FreeSuperblockManager & superblock_manager;
  StateFreeSuperblockManagerTest() : superblock_manager(state.superblock_manager_){
  }
};

TEST(FreeSuperblockManagerTest, return_superblock_overflow) {
	FreeSuperblockManager superblock_manager;

	for (size_t i = 0; i < 2 * kMaxFreeSuperblocks; i++) {
		Superblock *superblock = Superblock::Make();
		superblock_manager.AddSuperblock(new(superblock) Superblock);
	}
	ASSERT_EQ(superblock_manager.superblock_count(), kMaxFreeSuperblocks);
}

TEST(FreeSuperblockManagerTest, return_superblock_count) {
	FreeSuperblockManager superblock_manager;

	size_t old_superblocks_count = superblock_manager.superblock_count();
	ASSERT_TRUE(old_superblocks_count < kMaxFreeSuperblocks);
	size_t n_superblocks = kMaxFreeSuperblocks / 2;
	superblock_manager.MapNewSuperblocks(n_superblocks);
	ASSERT_EQ(superblock_manager.superblock_count(), n_superblocks + old_superblocks_count);
}

TEST(FreeSuperblockManagerTest, get_superblock_from_empty) {
	FreeSuperblockManager superblock_manager;
	for (size_t i = 0; i < 2 * kMaxFreeSuperblocks; i++) {
		Superblock *ptr = superblock_manager.GetSuperblock();
		munmap(ptr, sizeof(Superblock));
	}
}

TEST(FreeSuperblockManagerTest, memory_is_aligned) {
	FreeSuperblockManager superblock_manager;
	superblock_manager.MapNewSuperblocks(1);
	ASSERT_TRUE(reinterpret_cast<size_t>(superblock_manager.superblock_stack_.Pop()) % sizeof(Superblock) == 0);
}


TEST_F(StateFreeSuperblockManagerTest, return_superblock_overflow) {
  for (size_t i = 0; i < 2 * kMaxFreeSuperblocks; i++) {
    Superblock *superblock = Superblock::Make();
    superblock_manager.AddSuperblock(new(superblock) Superblock);
  }
  ASSERT_EQ(superblock_manager.superblock_count(), kMaxFreeSuperblocks);
}

TEST_F(StateFreeSuperblockManagerTest, return_superblock_count) {
  size_t old_superblocks_count = superblock_manager.superblock_count();
  ASSERT_TRUE(old_superblocks_count < kMaxFreeSuperblocks);
  size_t n_superblocks = kMaxFreeSuperblocks / 2;
  superblock_manager.MapNewSuperblocks(n_superblocks);
  ASSERT_EQ(superblock_manager.superblock_count(), n_superblocks + old_superblocks_count);
}

TEST_F(StateFreeSuperblockManagerTest, get_superblock_from_empty) {
  for (size_t i = 0; i < 2 * kMaxFreeSuperblocks; i++) {
    Superblock *ptr = superblock_manager.GetSuperblock();
    munmap(ptr, sizeof(Superblock));
  }
}

TEST_F(StateFreeSuperblockManagerTest, memory_is_aligned) {
  superblock_manager.MapNewSuperblocks(1);
  ASSERT_TRUE(reinterpret_cast<size_t>(superblock_manager.superblock_stack_.Pop()) % sizeof(Superblock) == 0);
}

#endif // HOARD_USE_SUPERBLOCK_MANAGER


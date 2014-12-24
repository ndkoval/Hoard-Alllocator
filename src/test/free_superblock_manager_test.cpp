#include <gtest/gtest.h>

//#include "Superblock.h"
#define private public

#include "FreeSuperblockManager.h"

#undef private

using namespace hoard;

TEST(free_superblock_manager, return_superblock_overflow) {
	FreeSuperblockManager freeSuperblockManager;

	for (size_t i = 0; i < 2 * kMaxFreeSuperblocks; i++) {
		Superblock *ptr =
				reinterpret_cast<Superblock *> (mmapAnonymous(sizeof(Superblock)));
		freeSuperblockManager.AddSuperblock(new(ptr) Superblock);
	}
	ASSERT_EQ(freeSuperblockManager.superblock_count_, kMaxFreeSuperblocks);
}

TEST(free_superblock_manager, return_superblock_count) {
	FreeSuperblockManager freeSuperblockManager;

	size_t old_superblocks_count = freeSuperblockManager.superblock_count_;
	ASSERT_TRUE(old_superblocks_count < kMaxFreeSuperblocks);
	size_t n_superblocks = kMaxFreeSuperblocks / 2;
	freeSuperblockManager.MapNewSuperblocks(n_superblocks);
	ASSERT_EQ(freeSuperblockManager.superblock_count_, n_superblocks + old_superblocks_count);
}

TEST(free_superblock_manager, get_superblock_from_empty) {
	FreeSuperblockManager freeSuperblockManager;
	for (size_t i = 0; i < 2 * kMaxFreeSuperblocks; i++) {
		Superblock *ptr = freeSuperblockManager.GetSuperblock();
		munmap(ptr, sizeof(Superblock));
	}
}

TEST(free_superblock_manager, memory_is_aligned) {
	FreeSuperblockManager freeSuperblockManager;
	freeSuperblockManager.MapNewSuperblocks(1);
	ASSERT_TRUE(reinterpret_cast<size_t>(freeSuperblockManager.superblock_stack_.Pop()) % sizeof(Superblock) == 0);
}
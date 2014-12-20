#include <gtest/gtest.h>

//#include "Superblock.h"
#define private public

#include "FreeSuperblockManager.h"

#undef private

namespace hoard {

TEST(free_superblock_manager, return_superblock_overflow) {
	FreeSuperblockManager<kSuperblockSize> freeSuperblockManager;

	for (size_t i = 0; i < 2 * kMaxFreeSuperblocks; i++) {
		Superblock<kSuperblockSize> *ptr =
				reinterpret_cast<Superblock<kSuperblockSize> *> (mmapAnonymous(sizeof(Superblock<kSuperblockSize>)));
		freeSuperblockManager.AddSuperblock(new(ptr) Superblock<kSuperblockSize>);
	}
	ASSERT_EQ(freeSuperblockManager.superblock_count_, kMaxFreeSuperblocks);
}

TEST(free_superblock_manager, return_superblock_count) {
	FreeSuperblockManager<kSuperblockSize> freeSuperblockManager;

	size_t old_superblocks_count = freeSuperblockManager.superblock_count_;
	ASSERT_TRUE(old_superblocks_count < kMaxFreeSuperblocks);
	size_t n_superblocks = kMaxFreeSuperblocks / 2;
	freeSuperblockManager.MapNewSuperblocks(n_superblocks);
	ASSERT_EQ(freeSuperblockManager.superblock_count_, n_superblocks + old_superblocks_count);
}

TEST(free_superblock_manager, get_superblock_from_empty) {
	FreeSuperblockManager<kSuperblockSize> freeSuperblockManager;
	for (size_t i = 0; i < 2 * kMaxFreeSuperblocks; i++) {
		Superblock<kSuperblockSize> *ptr = freeSuperblockManager.GetSuperblock();
		munmap(ptr, sizeof(Superblock<kSuperblockSize>));
	}
}

}
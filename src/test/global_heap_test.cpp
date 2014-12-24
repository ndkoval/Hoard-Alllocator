#include <gtest/gtest.h>
#include <GlobalHeap.h>

#define private public

#undef private

using namespace hoard;

namespace {
size_t kBlockSize = 16;
}

TEST(global_heap, get_superblock_from_empty) {
	FreeSuperblockManager manager;
	GlobalHeap globalHeap(manager, kBlockSize);
	Superblock *superblock = globalHeap.GetSuperblock();
	ASSERT_NE(superblock, nullptr);
}

TEST(global_heap, owner_is_heap_after_get) {
	FreeSuperblockManager manager;
	GlobalHeap globalHeap(manager, kBlockSize);
	Superblock *superblock = globalHeap.GetSuperblock();
	ASSERT_EQ(superblock->header().owner(), &globalHeap);
}

TEST(global_heap, owner_changed_on_unempty_superblock_after_add) {
	FreeSuperblockManager manager;
	GlobalHeap globalHeap(manager, kBlockSize);
	Superblock superblock;
	superblock.header().Init(kBlockSize);
	superblock.header().Alloc();
	globalHeap.AddSuperblock(&superblock);
	ASSERT_EQ(superblock.header().owner(), &globalHeap);
}
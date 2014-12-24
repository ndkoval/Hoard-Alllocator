#include <gtest/gtest.h>
#include <GlobalHeap.h>
#include <FreeSuperblockManager.h>

#define private public

#include "GlobalHeap.h"

#undef private

using namespace hoard;

class hoard::GlobalHeap;
class hoard::FreeSuperblockManager;

TEST(global_heap, get_superblock_from_empty) {
    GlobalHeap globalHeap(new FreeSuperblockManager());
    Superblock* superblock = globalHeap.GetSuperblock();
    ASSERT_NE(superblock, nullptr);
}
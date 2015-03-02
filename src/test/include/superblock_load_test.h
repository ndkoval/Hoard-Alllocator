#ifndef HOARD_TEST_SUPERBLOCK_LOAD_H
#define HOARD_TEST_SUPERBLOCK_LOAD_H


#include <gtest/gtest.h>
#include <Superblock.h>
#include <cstring>
#include <vector>
#include <algorithm>

namespace hoard {
void SuperblockLoadTest(Superblock & superblock) {
  SuperblockHeader &header = superblock.header();
  EXPECT_TRUE(header.empty());
  for (int i_load_iteration = 0; i_load_iteration < 3; ++i_load_iteration) {

    std::vector<char *> blocks;
    //allocate, modify
    for (size_t j_block_iteration = 0; j_block_iteration < header.size(); ++j_block_iteration) {
      EXPECT_EQ(j_block_iteration, header.blocks_allocated());
      char *block;
      EXPECT_NO_FATAL_FAILURE(block = (char *) header.Alloc());
      blocks.push_back(block);
      EXPECT_NO_FATAL_FAILURE(std::memset(block, 1, header.one_block_size()));
    }
    std::random_shuffle(blocks.begin(), blocks.end());
    //random modify
    for (auto block : blocks) {
      EXPECT_NO_FATAL_FAILURE(std::memset(block, 1, header.one_block_size()));
    }
    //partial free
    for (size_t j_block_iteration = 0; j_block_iteration < header.size() / 2; ++j_block_iteration) {
      char *block = blocks.back();
      blocks.pop_back();
      EXPECT_NO_FATAL_FAILURE(header.Free(block));
    }
    //random modify
    for (auto block : blocks) {
      EXPECT_NO_FATAL_FAILURE(std::memset(block, 1, header.one_block_size()));
    }
    //total free
    for (auto block : blocks) {
      EXPECT_NO_FATAL_FAILURE(header.Free(block));
    }
    EXPECT_TRUE(header.empty());
  }
}
}

#endif //HOARD_TEST_SUPERBLOCK_LOAD_H

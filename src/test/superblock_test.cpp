#include <gtest/gtest.h>

#define private public
#include <Superblock.h>
#undef private
#include <superblock_load_test.h>

using namespace hoard;

namespace {
const size_t kBlockSize = 32;
}

class SuperblockUninitedTest : public ::testing::Test {
public:

  Superblock &superblock;
  SuperblockHeader &header;
protected:

  SuperblockUninitedTest()
      : Test(),
        superblock(*Superblock::Make()),
        header(superblock.header()) {
    new (&superblock) Superblock();
  }

  virtual void TearDown() {
    munmap(&superblock, kSuperblockSize);

  }



};

class SuperblockInitedTest : public SuperblockUninitedTest {

protected:
  virtual void SetUp() override {
    header.Init(kBlockSize);
  }
};

TEST_F(SuperblockUninitedTest, ValidTest) {
  EXPECT_FALSE(header.valid());
  EXPECT_EQ(superblock, *header.GetSuperblock());
}

TEST_F(SuperblockUninitedTest, UnderflowTest) {
  EXPECT_DEATH(header.Free(nullptr), ".*");
}


TEST_F(SuperblockInitedTest, ValidTest) {
  EXPECT_TRUE(header.valid());
  EXPECT_TRUE(header.empty());
  EXPECT_EQ(superblock, *header.GetSuperblock());
  EXPECT_EQ(header.one_block_size(), kBlockSize);
  EXPECT_EQ(header.size(),
      (reinterpret_cast<char *>(&header) + kSuperblockSize - header.blocks_start_) / header.one_block_size());
}

TEST_F(SuperblockInitedTest, AllocTest) {
  char *block = (char *) header.Alloc();
  EXPECT_NO_FATAL_FAILURE(std::memset(block, 1, kBlockSize));
  EXPECT_EQ(header.blocks_allocated(), 1);
  EXPECT_FALSE(header.empty());
}

TEST_F(SuperblockInitedTest, FreeTest) {
  char *block;
  EXPECT_NO_FATAL_FAILURE(block = (char *) header.Alloc());
  EXPECT_NO_FATAL_FAILURE(std::memset(block, 1, kBlockSize));
  EXPECT_EQ(header.blocks_allocated(), 1);
  EXPECT_FALSE(header.empty());
  EXPECT_NO_FATAL_FAILURE(header.Free(block));
  EXPECT_TRUE(header.empty());
}


TEST_F(SuperblockInitedTest, OverflowTest) {
  for (size_t i = 0; i < header.size(); ++i) {
    EXPECT_EQ(i, header.blocks_allocated());
    char *block;
    EXPECT_NO_FATAL_FAILURE(block = (char *) header.Alloc());
    EXPECT_NO_FATAL_FAILURE(std::memset(block, 1, kBlockSize));
  }
  EXPECT_DEATH(header.Alloc(), ".*");
}

TEST_F(SuperblockInitedTest, UnderflowTest) {
  std::vector<char *> blocks;
  for (size_t i = 0; i < header.size(); ++i) {
    EXPECT_EQ(i, header.blocks_allocated());
    char *block;
    EXPECT_NO_FATAL_FAILURE(block = (char *) header.Alloc());
    blocks.push_back(block);
    EXPECT_NO_FATAL_FAILURE(std::memset(block, 1, kBlockSize));
  }
  std::random_shuffle(blocks.begin(), blocks.end());
  char *some_block = blocks.front();
  for (auto block : blocks) {
    EXPECT_NO_FATAL_FAILURE(header.Free(block));
  }
  EXPECT_DEATH(header.Free(some_block), ".*");
}


TEST_F(SuperblockInitedTest, LoadTest) {
  SuperblockLoadTest(superblock);
}
TEST_F(SuperblockInitedTest, ReuseLoadTest) {
  SuperblockLoadTest(superblock);
  header.Init(kBlockSize*8);
  SuperblockLoadTest(superblock);
  header.Init(kBlockSize*32);
  SuperblockLoadTest(superblock);

}
TEST_F(SuperblockInitedTest, LargeBlockReuseTest) {
  SuperblockLoadTest(superblock);
  header.Init(kSuperblockSize/4);
  SuperblockLoadTest(superblock);
  header.Init(kSuperblockSize/2);
  SuperblockLoadTest(superblock);
  header.Init(kBlockSize);
  SuperblockLoadTest(superblock);
}





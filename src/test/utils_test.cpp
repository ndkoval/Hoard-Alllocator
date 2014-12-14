#include <gtest/gtest.h>
#include "utils.h"
using namespace hoard;

TEST(utils, IsPowerOf2) {
    EXPECT_TRUE(IsPowerOf2(512 * 1024));
    EXPECT_FALSE(IsPowerOf2(3));
}

TEST(utils, IsValidAlignment) {
    EXPECT_TRUE(IsValidAlignment(512 * 1024));
    EXPECT_FALSE(IsValidAlignment(sizeof(void *) / 2));
}

TEST(uitls, RoundUp) {
    EXPECT_EQ(4, RoundUp(3, 2));
    EXPECT_EQ(4, RoundUp(4, 2));
}

TEST(mmap, mmapAnonymous) {
    const size_t mmaped_size = kPageSize * 4 + 13;
    auto *result = mmapAnonymous(mmaped_size);
    EXPECT_NE(nullptr, result);
    EXPECT_NO_FATAL_FAILURE(memset(result, 0xAF, mmaped_size));
    EXPECT_EQ(0, munmap(result, mmaped_size));
}

TEST(mmapHint, mmapAnonymous) {
    const size_t mmaped_size = kPageSize * 4 + 13;
    auto *result = mmapAnonymous(reinterpret_cast<void*>(1234567), mmaped_size);
    EXPECT_NE(nullptr, result);
    EXPECT_NO_FATAL_FAILURE(memset(result, 0xAF, mmaped_size));
    EXPECT_EQ(0, munmap(result, mmaped_size));
}

TEST(mmapAlligned, small_allignment) {
    const size_t mmaped_size = kPageSize * 4 + 13;
    const int alignment = 512;
    void *result;
    ASSERT_NO_FATAL_FAILURE(result = mmapAligned(mmaped_size, alignment));
    EXPECT_TRUE(reinterpret_cast<size_t>(result) % alignment == 0);
    EXPECT_NE(nullptr, result);
    EXPECT_NO_FATAL_FAILURE(memset(result, 0xAF, mmaped_size));
    EXPECT_EQ(0, munmap(result, mmaped_size));
}

TEST(mmapAlligned, big_allignment) {
    const size_t mmaped_size = kPageSize * 4 + 13;
    const int alignment = kPageSize * 16;
    void *result;
    ASSERT_NO_FATAL_FAILURE(result = mmapAligned(mmaped_size, alignment));
    EXPECT_TRUE(reinterpret_cast<size_t>(result) % alignment == 0);
    EXPECT_NE(nullptr, result);
    EXPECT_NO_FATAL_FAILURE(memset(result, 0xAF, mmaped_size));
    EXPECT_EQ(0, munmap(result, mmaped_size));
}


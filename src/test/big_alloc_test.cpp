#include <string.h>
#include "../hoard_allocator/Internals.h"
#include "../gtest/include/gtest/gtest.h"

class BigAlloc : public ::testing::Test {
	void SetUp() {
		hoard::InternalFree(nullptr);
	};
};


TEST_F(BigAlloc, alloc_and_free) {
	char *ptr = (char *) hoard::BigAlloc(hoard::kMaxBlockSize, hoard::kDefaultAlignment);
	ASSERT_NE(ptr, nullptr);
	ptr[0] = 'c';
	ASSERT_EQ(ptr[0], 'c');
	ASSERT_TRUE(hoard::BigFree(ptr));
}

TEST_F(BigAlloc, realloc) {
	char *ptr = (char *) hoard::BigAlloc(hoard::kMaxBlockSize, hoard::kDefaultAlignment);
	ptr[0] = 'c';
	ptr = (char *) hoard::InternalRealloc(ptr, hoard::kMaxBlockSize * 2);
	ASSERT_EQ(ptr[0], 'c');
	ptr[hoard::kMaxBlockSize * 2 - 1] = 'c';
	ASSERT_TRUE(hoard::BigFree(ptr));
}
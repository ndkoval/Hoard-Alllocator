#include <string.h>
#include "../hoard_allocator/Internals.h"
#include "../gtest/include/gtest/gtest.h"

class BigAlloc : public ::testing::Test {
	void SetUp() {
		hoard::InternalFree(nullptr);
	};
};

TEST_F(BigAlloc, alloc_and_free) {
}

TEST_F(BigAlloc, realloc) {
}
#include "cstdlib"
#include "ldpreload_testing.h"

int main() {
	auto ptr = nullptr;
	EXPECT_TRUE(ptr == nullptr);
	EXPECT_FALSE(ptr);
	ASSERT_TRUE(ptr == nullptr);
	ASSERT_FALSE(ptr);

	return TestResult();
}
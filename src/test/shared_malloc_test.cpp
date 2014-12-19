#include "ldpreload_testing.h"


namespace hoard {
void *SharedMalloc(size_t);
}


int main() {
	hoard::println("malloc result: ", malloc(32));
	hoard::println("shared_alloc result: ", hoard::SharedMalloc(32));
	EXPECT_TRUE(hoard::SharedMalloc(32) == nullptr);
	return TestResult();
}
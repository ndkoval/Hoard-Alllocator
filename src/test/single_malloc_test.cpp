#include "cstdlib"
#include "noalloc_testing.h"

int main() {
    auto * ptr =  malloc(16);
    EXPECT_TRUE(ptr == nullptr);
    EXPECT_FALSE(ptr);
    ASSERT_TRUE(ptr == nullptr);
    ASSERT_FALSE(ptr);
    free(ptr);

    return TestResult();
}
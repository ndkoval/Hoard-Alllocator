#include <string.h>
#include "noalloc_testing.h"

int main() {
	void *ptr = malloc(1024);
	EXPECT_FALSE(ptr == nullptr);

	void *ptr2 = malloc(1024);
	memcpy(ptr, ptr2, 1024);

	ptr = realloc(ptr, 2048);

	free(ptr2);
	free(ptr);

	return TestResult();
}
#include "cstdlib"
#include "noalloc_testing.h"
#include "AllocFreeHashMap.h"

int main() {
	constexpr size_t max_val = 100000;
	hoard::AllocFreeHashMap my_map{};

	for (int k = 0; k < 3; k++) {

		for (size_t i = 0; i < max_val; i++) {
			hoard::AllocFreeHashMap::key_type key = reinterpret_cast<hoard::AllocFreeHashMap::key_type>( i);
			ASSERT_FALSE(my_map.Contains(key));
			my_map.Add(key, i + 500);
			ASSERT_TRUE(my_map.Contains(key));
		}

		for (size_t i = 0; i < max_val; i++) {
			hoard::AllocFreeHashMap::key_type key = (hoard::AllocFreeHashMap::key_type) i;
			ASSERT_TRUE(my_map.Contains(key));
			ASSERT_TRUE(my_map.Get(key) == i + 500);
			ASSERT_TRUE(my_map.Remove(key));
			ASSERT_FALSE(my_map.Remove(key));
		}
		ASSERT_TRUE(my_map.size() == 0)

	}

	return TestResult();
}
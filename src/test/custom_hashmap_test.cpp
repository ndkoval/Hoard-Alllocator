#include <map>
#include "gtest/gtest.h"
#include "AllocFreeHashMap.h"
#include "tracing.h"

TEST(alloc_free_hashmap, no_alloc) {
  auto stop_trace = hoard::StopTraceGuard();
	constexpr size_t max_val = 1023;
	hoard::AllocFreeHashMap my_map{};

	for (int k = 0; k < 3; k++) {

		for (size_t i = 1; i < max_val; i++) {

			hoard::AllocFreeHashMap::key_type key = reinterpret_cast<hoard::AllocFreeHashMap::key_type>( i);
//        hoard::print("key is: ", key, "\n");
      if(i % 5 == 0) {
        my_map.Add(key, i + 500);
      } else {
        my_map.Set(key, i + 500);
      }
      if(k % 2 == 1) {
        my_map.Set(key, i + 1);
        my_map.Set(key, i + 500);
      }
			ASSERT_TRUE(my_map.Contains(key));

			for (size_t j = 1; j <= i; j++) {
				hoard::AllocFreeHashMap::key_type key2 = reinterpret_cast<hoard::AllocFreeHashMap::key_type>( j);
				if (!my_map.Contains(key2)) {
//                hoard::print("after add: ", i, " lost: ", j, "\n");
//                my_map.PrintState();
					ASSERT_TRUE(my_map.Contains(key2));
				}
			}
			for (size_t j = i + 1; j <= max_val; j++) {
				hoard::AllocFreeHashMap::key_type key2 = reinterpret_cast<hoard::AllocFreeHashMap::key_type>( j);
				if (my_map.Contains(key2)) {
//                hoard::print("after add: ", i, " added: ", j, "\n");
					ASSERT_TRUE(!my_map.Contains(key2));
				}
			}
//        hoard::print("\n\n new print ", i," \n \n");

		}

//        my_map.PrintState();
//    hoard::print("hi from gtest!!!\n");
		for (size_t i = 1; i < max_val; i++) {
//        ASSERT_EQ(my_map.size(), max_val - i);
//        hoard::print("beg\n");
			hoard::AllocFreeHashMap::key_type key = (hoard::AllocFreeHashMap::key_type) i;
			ASSERT_TRUE(my_map.Contains(key));

//        hoard::print("after cont\n");
//        hoard::print("iter: ", (size_t)  i, " val: ", my_map.get(key), "\n");
			ASSERT_TRUE(my_map.Get(key) == i + 500);
//        hoard::print(i, my_map.get(key));
			ASSERT_TRUE(my_map.Remove(key));
			for (size_t j = 1; j <= i; j++) {
				hoard::AllocFreeHashMap::key_type key2 = reinterpret_cast<hoard::AllocFreeHashMap::key_type>( j);
				if (my_map.Contains(key2)) {
//                hoard::print("after dell: ", i, " added: ", j,  "\n");
					ASSERT_TRUE(!my_map.Contains(key2));
				}
			}
			for (size_t j = i + 1; j < max_val; j++) {
				hoard::AllocFreeHashMap::key_type key2 = reinterpret_cast<hoard::AllocFreeHashMap::key_type>( j);
				if (!my_map.Contains(key2)) {
//                hoard::print("after del: ", i, " lost: ", j, "\n");
					ASSERT_TRUE(my_map.Contains(key2));
				}
			}
			ASSERT_FALSE(my_map.Remove(key));

		}
	}


}



#include <map>
#include "gtest/gtest.h"
#include "no_alloc_hashmap.h"

TEST(no_alloc_hashmap, alloc)
{
    constexpr size_t max_val = 10;
    hoard::no_alloc_hashmap my_map{};
    for(size_t i = 1; i < max_val; i++) {

        hoard::no_alloc_hashmap::key_type key = reinterpret_cast<hoard::no_alloc_hashmap::key_type>( i);
        hoard::print("key is: ", key);
        my_map.add(key , i + 500);
        ASSERT_TRUE(my_map.contains(key));
        hoard::print("\n\n new print ", i," \n \n");
        my_map.print_state();

    }

    hoard::print("hi from gtest!!!\n");
    for(int i = 0; i < max_val; i++) {
//        ASSERT_EQ(my_map.size(), max_val - i);
        hoard::print("beg");
        hoard::no_alloc_hashmap::key_type key = (hoard::no_alloc_hashmap::key_type) i;
        ASSERT_TRUE(my_map.contains(key));
        hoard::print("after cont");
        hoard::print((size_t)  i, my_map.get(key));
        ASSERT_TRUE(my_map.get(key) == i + 500);
        hoard::print(i, my_map.get(key));
        ASSERT_TRUE(my_map.remove(key));
        ASSERT_FALSE(my_map.remove(key));

    }



}



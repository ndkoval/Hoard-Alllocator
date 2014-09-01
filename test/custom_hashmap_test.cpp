#include <map>
#include "gtest/gtest.h"
#include "no_alloc_hashmap.h"

TEST(no_alloc_hashmap, alloc)
{
    constexpr size_t max_val = 1023 ;
    hoard::no_alloc_hashmap my_map{};

    void * keys[max_val];
    size_t values[max_val];



for(int k = 0; k < 3; k++) {

    for(size_t i = 0; i < max_val; i++) {

        hoard::no_alloc_hashmap::key_type key = reinterpret_cast<hoard::no_alloc_hashmap::key_type>( i);
        hoard::print("key is: ", key, "\n");
        my_map.add(key , i + 500);
        ASSERT_TRUE(my_map.contains(key));

        for(size_t j = 0; j <= i; j++) {
            hoard::no_alloc_hashmap::key_type key2 = reinterpret_cast<hoard::no_alloc_hashmap::key_type>( j);
            if(!my_map.contains(key2)) {
                hoard::print("after add: ", i, " lost: ", j, "\n");
                my_map.print_state();
                ASSERT_TRUE(my_map.contains(key2));
            }
        }
        for(size_t j = i + 1; j <= max_val; j++) {
            hoard::no_alloc_hashmap::key_type key2 = reinterpret_cast<hoard::no_alloc_hashmap::key_type>( j);
            if(my_map.contains(key2)) {
                hoard::print("after add: ", i, " added: ", j, "\n");
                ASSERT_TRUE(!my_map.contains(key2));
            }
        }
        hoard::print("\n\n new print ", i," \n \n");

    }

        my_map.print_state();
    hoard::print("hi from gtest!!!\n");
    for(size_t i = 0; i < max_val; i++) {
//        ASSERT_EQ(my_map.size(), max_val - i);
        hoard::print("beg\n");
        hoard::no_alloc_hashmap::key_type key = (hoard::no_alloc_hashmap::key_type) i;
        ASSERT_TRUE(my_map.contains(key));

        hoard::print("after cont\n");
        hoard::print("iter: ", (size_t)  i, " val: ", my_map.get(key), "\n");
        ASSERT_TRUE(my_map.get(key) == i + 500);
        hoard::print(i, my_map.get(key));
        ASSERT_TRUE(my_map.remove(key));
        for(size_t j = 0; j <= i; j++) {
            hoard::no_alloc_hashmap::key_type key2 = reinterpret_cast<hoard::no_alloc_hashmap::key_type>( j);
            if(my_map.contains(key2)) {
                hoard::print("after dell: ", i, " added: ", j,  "\n");
                ASSERT_TRUE(!my_map.contains(key2));
            }
        }
        for(size_t j = i + 1; j < max_val; j++) {
            hoard::no_alloc_hashmap::key_type key2 = reinterpret_cast<hoard::no_alloc_hashmap::key_type>( j);
            if( !my_map.contains(key2)) {
                hoard::print("after del: ", i, " lost: ", j, "\n");
                ASSERT_TRUE(my_map.contains(key2));
            }
        }
        ASSERT_FALSE(my_map.remove(key));

    }
    }



}



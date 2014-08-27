#ifndef NO_ALLOC_HASHMAP_H
#define NO_ALLOC_HASHMAP_H
#include "hoard_constants.h"
#include "utils.h"
#include "tracing.h"

/*
 * void * -> size_t hashmap
 * uses no malloc, but mmap for entery table allocating
 * used double hashing for collision resolution
 * can store multple values for one key, but order of get and remove not defined
 */

namespace hoard {

class no_alloc_hashmap
{

public:

    typedef void * key_type;
    typedef size_t value_type;
    constexpr static value_type NO_SUCH_KEY = (value_type) -1;
private:





    size_t MINIMUM_TABLE_SIZE = PAGE_SIZE; // multiple PAGE_SIZE
    constexpr static size_t FIXED_POINT_SHIFT = 8;
    constexpr static size_t FULLNESS_THRESHOLD = (1 << FIXED_POINT_SHIFT) / 2; // talbe with n/2 entryes should be expanded
    constexpr static size_t EMPTYNESS_THRESHOLD = (1 << FIXED_POINT_SHIFT) / 8; // talbe with n/8 entryes should be shrinked
    constexpr static size_t EXPASION_RATE = 2;//power of 2
    constexpr static size_t SHRINKING_RATE = 2; // power of 2


    struct table_entry {
        key_type key;
        value_type value;

        table_entry(key_type key, value_type value) : key(key), value(value){}
        table_entry( const table_entry&) = default;

        bool empty() {
            return key == 0 && value == 0;
        }
        void clear() {
            value = 0;
            key = nullptr;
        }
    };


    static_assert(is_power_of_2(sizeof(table_entry)), "table_entry should be power of 2 size for having power of 2 entryes in table"); // second hashfunction is odd so entry_num will be coprime with it value

    table_entry * _table;
    bool * _deleted;
    size_t _table_entry_num;
    size_t _table_entry_size;
    size_t _table_mem_size;
    size_t _deleted_mem_size;
    size_t _hint; // last finded index.


    constexpr inline static size_t first_hash(key_type key) {
        return ((((((size_t)key) ^(((size_t) key) >> (sizeof(key_type) * 4))) * 67867967) + 122949823 ) % 32416190071ll);
    }

    constexpr inline static size_t second_hash(key_type key) {
        return ((((((size_t)key) ^(((size_t) key) >> (sizeof(key_type) * 4))) * 179426339 ) + 6461350277 ) % 32416190071ll) | 0b1 ; // always odd
    }

    inline size_t get_index(size_t hash) {
        return hash & (_table_entry_size -1);
    }
    inline void set(size_t index, const key_type & key, const value_type & value) {
        _table[index] = table_entry(key, value);
        _deleted[index] = false;

    }

    inline bool hint_is_valid(key_type key) {
        return _table[_hint].key == key && !_deleted[_hint] && !_table[_hint].empty();
    }

    void internal_add(const key_type & key, const value_type & value){
        size_t hash1 = first_hash(key);
        size_t index = get_index(hash1);
        if(_table[index].empty() || _deleted[index]){
            set(index, key, value);
        } else{
            size_t hash2 = second_hash(key);
            size_t curent_hash = hash1;
            do {
                curent_hash += hash2;
                index = get_index(curent_hash);

            } while(_table[index].empty() || _deleted[index]);
            set(index, key, value);
        }

    }
    bool internal_remove(const key_type & key) { // true if key was in table
        int index = internal_find(key);
        if(index == -1) {
            return false;
        } else{
            _deleted[index] = true;
            return true;
        }

    }

    int internal_find(const key_type & key) {
        if(hint_is_valid(key)) {
            return _hint;
        }
        size_t current_hash = first_hash(key), hash2  = second_hash(key);
        size_t index = get_index(current_hash);
        while(!_table[index].empty()) {
            if(!_deleted[index] && _table[index].key == key) {
                _hint = index;
                return index;
            } else{
                current_hash += hash2;
                index = get_index(current_hash);
            }
        }
        return -1;

    }

    inline void init_new_table(size_t new_table_mem_size) {
        assert(is_power_of_2(new_table_mem_size));
        _table_mem_size = new_table_mem_size;
        _table_entry_size = _table_mem_size / sizeof(table_entry);
        _deleted_mem_size = round_up(_table_entry_size * sizeof(bool), PAGE_SIZE);
        _deleted =(bool *) mmap_anonymous(_deleted_mem_size);
        _table = (table_entry*) mmap_anonymous(new_table_mem_size);
        if(_table == MAP_FAILED || _deleted == MAP_FAILED) {
           print("hoard-allocator error: big allocation hashmap resize failed\n");
           std::abort();
        }
    }
    inline void resize(size_t new_table_mem_size) {
        assert(new_table_mem_size > _table_entry_num * sizeof(table_entry));
        _hint = 0;
        table_entry * old_table = _table;
        size_t old_table_entry_size = _table_entry_size, old_table_mem_size = _table_mem_size;
        size_t old_deleted_mem_size = _deleted_mem_size;
        bool * old_deleted = _deleted;
        init_new_table(new_table_mem_size);
        for(int i = 0; i < old_table_entry_size; i++) {
            if(!old_table[i].empty() && !old_deleted[i]) {
                internal_add(old_table[i].key, old_table[i].value);
            }
        }

       assert(munmap(old_table, old_table_mem_size) == 0);
       assert(munmap(old_deleted, old_deleted_mem_size) == 0);
    }

public:
    no_alloc_hashmap() : _table_entry_num(0), _hint(0) {
        init_new_table(MINIMUM_TABLE_SIZE);
    }

    void add(key_type key, value_type value) {
        internal_add(key, value);
        _table_entry_num++;
        if(_table_entry_num >= (FULLNESS_THRESHOLD * _table_entry_size) >> FIXED_POINT_SHIFT ) {
            resize(_table_mem_size * EXPASION_RATE);
        }


    }

    bool contains(const key_type & key) {
        return internal_find(key) != -1;

    }

    value_type get(key_type key) {
        int index = internal_find(key);
        if(index == -1) {
            return NO_SUCH_KEY;
        } else {
            return _table[index].value;
        }


    }

    bool remove(key_type key) { // true if key was in table
        bool result = internal_remove(key);
        if(result) {
            _table_entry_num--;
            if((_table_entry_num <= (EMPTYNESS_THRESHOLD * _table_entry_size) >> FIXED_POINT_SHIFT) && _table_mem_size > MINIMUM_TABLE_SIZE) {
                resize(_table_mem_size/SHRINKING_RATE);
            }
        }
        return result;

    }
    size_t size() {
        return _table_entry_num;
    }


    void print_state() {
        for(int i = 0; i < _table_entry_size; i++) {
            if(!_table[i].empty()) {
                print("cell: ", i, " key: ", _table[i].key, " value: ", _table[i].value, " deleted: ", _deleted[i], " hash1: ", first_hash(_table[i].key), " hash2: ", second_hash(_table[i].key), "\n" );
            }
        }
    }





};
}


#endif // NO_ALLOC_HASHMAP_H

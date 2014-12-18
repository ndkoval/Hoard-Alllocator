#ifndef NO_ALLOC_HASHMAP_H
#define NO_ALLOC_HASHMAP_H

#include "hoard_constants.h"
#include "utils.h"
#include "tracing.h"
#include <functional>

/*
 * void * -> size_t hashmap
 * uses no malloc, but mmap for entry table allocating
 * used double hashing for collision resolution
 * can store multiply values for one key, but order of get and Remove not defined
 */

namespace hoard {

class AllocFreeHashMap {

public:
  typedef void *key_type;
  typedef size_t value_type;
  constexpr static value_type kNoSuchKey = (value_type) -1;
private:

  const size_t kMinimumTableSize = kPageSize; // multiple kPageSize, can't be static;
  constexpr static size_t kFixedPointShift = 8;
  constexpr static size_t kFullnessThreshold = (1 << kFixedPointShift) / 2; // table with n/2 entries should be expanded
  constexpr static size_t kEmptynessThreshold = (1 << kFixedPointShift) / 8; // table with n/8 entries should be shrinked
  constexpr static size_t kEmptyCellsThreshold = (1 << kFixedPointShift) / 16; // table with less than n/16 empty cells (deleted != empty) will be rehashed in same size
  static_assert(kEmptyCellsThreshold <= kEmptynessThreshold, "invalid values");
  constexpr static size_t kExpansionRate = 2;
  //power of 2
  constexpr static size_t kShrinkingRate = 2; // power of 2


  struct TableEntry {
    key_type key;
    value_type value;

    TableEntry(key_type key, value_type value) : key(key), value(value) {
    }

    TableEntry(const TableEntry &) = default;

    bool empty() {
      return key == nullptr && value == 0;
    }

    void clear() {
      value = 0;
      key = nullptr;
    }
  };

  // second hashf unction is odd so entry_num will be coprime with it value
  static_assert(IsPowerOf2(sizeof(TableEntry)), "TableEntry should be power of 2 size for having power of 2 entryes in table");

  TableEntry *table_;
  bool *deleted_;
  size_t table_entry_num_;
  size_t table_entry_size_;
  size_t table_mem_size_;
  size_t deleted_mem_size_;
  size_t hint_; // last founded Index.
  // if there are too small empty cells (not full or deleted), find can be too slow, even cycle, if table is full of not empty cells, and there is no such key in table
  size_t empty_cells_;


  inline static size_t FirstHash(key_type key) {
    return ((((((size_t) key) ^ (((size_t) key) >> (sizeof(key_type) * 4))) * 67867967) + 122949823) % 32416190071ll);
//          return std::hash<size_t>()((size_t) key);
  }

  inline static size_t SecondHash(key_type key) {
    return ((((((size_t) key) ^ (((size_t) key) >> (sizeof(key_type) * 4))) * 179426339) + 6461350277) % 32416190071ll) | 1; // always odd
  }

  inline size_t Index(size_t hash) {
    return hash & (table_entry_size_ - 1);
  }

  inline void Set(size_t index, const key_type &key, const value_type &value) {

    if (!deleted_[index]) {
      empty_cells_--;
    };
    table_[index] = TableEntry(key, value);
    deleted_[index] = false;

  }

  inline bool HintIsValid(key_type key) {
    return table_[hint_].key == key && !deleted_[hint_] && !table_[hint_].empty();
  }

  void InternalAdd(const key_type &key, const value_type &value) {
    hoard::trace("Page size is: ", hoard::kRealPageSize);
    trace("table_entry_size: ", table_entry_size_);
    size_t current_hash = FirstHash(key);
    size_t index = Index(current_hash);
    if (table_[index].empty() || deleted_[index]) {
      Set(index, key, value);
    } else {
      size_t hash2 = SecondHash(key);
      do {
        current_hash += hash2;
        index = Index(current_hash);

      } while (!table_[index].empty() && !deleted_[index]);
      Set(index, key, value);
    }
  }

  bool InternalRemove(const key_type &key) { // true if key was in table
    size_t index = InternalFind(key);
    if (index == kNoSuchKey) {
      return false;
    } else {
      deleted_[index] = true;
      return true;
    }

  }

  size_t InternalFind(const key_type &key) {
    if (HintIsValid(key)) {
      return hint_;
    }
    size_t current_hash = FirstHash(key), hash2 = SecondHash(key);
    for (size_t index = Index(current_hash); !table_[index].empty(); current_hash += hash2, index = Index(current_hash)) {
      if (!deleted_[index] && table_[index].key == key) {
        hint_ = index;
        return index;
      }
    }
    return kNoSuchKey;

  }

  inline void InitNewTable(size_t new_table_mem_size) {
    assert(IsPowerOf2(new_table_mem_size));
    table_mem_size_ = new_table_mem_size;
    table_entry_size_ = table_mem_size_ / sizeof(TableEntry);
    assert(table_entry_size_ > 0);
    empty_cells_ = table_entry_size_;
    deleted_mem_size_ = RoundUp(table_entry_size_ * sizeof(bool), kPageSize);
    deleted_ = (bool *) mmapAnonymous(deleted_mem_size_);
    table_ = (TableEntry *) mmapAnonymous(new_table_mem_size);
    if (table_ == MAP_FAILED || deleted_ == MAP_FAILED) {
      trace("hoard-allocator error: big allocation hashmap Resize failed\n");
      std::abort();
    }
  }

  inline void Resize(size_t new_table_mem_size) {
    assert(new_table_mem_size > table_entry_num_ * sizeof(TableEntry));
    hint_ = 0;
    TableEntry *old_table = table_;
    size_t old_table_entry_size = table_entry_size_, old_table_mem_size = table_mem_size_;
    size_t old_deleted_mem_size = deleted_mem_size_;
    bool *old_deleted = deleted_;
    size_t old_empty_cells = empty_cells_;
    InitNewTable(new_table_mem_size);
    size_t entry_counter = 0, empty_cell_counter = 0;
    for (size_t i = 0; i < old_table_entry_size; i++) {
      if (!old_table[i].empty() && !old_deleted[i]) {
        entry_counter++;
        InternalAdd(old_table[i].key, old_table[i].value);
        assert(Contains(old_table[i].key));
      } else if (old_table[i].empty()) {
        empty_cell_counter++;
      }

    }
    if (entry_counter != table_entry_num_) {
      hoard::print("added in Resize: ", entry_counter, "added total: ", table_entry_num_);
      assert(entry_counter == table_entry_num_);
    } else if (old_empty_cells != empty_cell_counter) {
      hoard::print("empty cells invariant lost. Expexted: ", old_empty_cells, " founded: ", empty_cell_counter, "\n");
      assert(old_empty_cells == empty_cell_counter);
    }

    assert(munmap(old_table, old_table_mem_size) == 0);
    assert(munmap(old_deleted, old_deleted_mem_size) == 0);
  }

public:
  AllocFreeHashMap() : table_entry_num_(0), hint_(0) {
    InitNewTable(kMinimumTableSize);
  }

  void Add(key_type key, value_type value) {
    InternalAdd(key, value);
    table_entry_num_++;
    if (table_entry_num_ >= (kFullnessThreshold * table_entry_size_) >> kFixedPointShift) {
      Resize(table_mem_size_ * kExpansionRate);
    } else if (empty_cells_ < (kEmptyCellsThreshold * table_entry_size_) >> kFixedPointShift) {
      Resize(table_mem_size_);
    }


  }

  bool Contains(const key_type &key) {
    return InternalFind(key) != kNoSuchKey;

  }

  value_type Get(key_type key) {
    size_t index = InternalFind(key);
    if (index == kNoSuchKey) {
      return kNoSuchKey;
    } else {
      return table_[index].value;
    }


  }

  bool Remove(key_type key) { // true if key was in table
    bool result = InternalRemove(key);
    if (result) {
      table_entry_num_--;
      if ((table_entry_num_ <= (kEmptynessThreshold * table_entry_size_) >> kFixedPointShift) && table_mem_size_ > kMinimumTableSize) {
        Resize(table_mem_size_ / kShrinkingRate);
      }
    }
    return result;

  }

  size_t size() {
    return table_entry_num_;
  }


  void PrintState() {
    trace("AllocFreeHashMap{\n");
    for (size_t i = 0; i < table_entry_size_; i++) {
      if (!table_[i].empty()) {
        size_t hash1 = FirstHash(table_[i].key), hash2 = SecondHash(table_[i].key);
        size_t cell_in_chain = 0;
        for (size_t k = 0; i != Index(hash1 + hash2 * k); cell_in_chain++, k++) {};
        trace("cell: ", i, " key: ", (size_t) table_[i].key, " value: ", table_[i].value, " deleted: ", deleted_[i], " hash1: ", hash1, " hash2: ", hash2, " Index 1: ",
            Index(hash1), " Index 2: ", Index(hash1 + hash2), " Index 3: ", Index(hash1 + 2 * hash2), " cell in chain: ", cell_in_chain, "\n");
      }
    }
    trace("}\n");
  }


};
}


#endif // NO_ALLOC_HASHMAP_H

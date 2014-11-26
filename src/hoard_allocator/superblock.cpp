#include "superblock.h"

void hoard::FreeBlockStack::Push(void *spaceForExtraBlock) {
  head_ = new(spaceForExtraBlock)  Block; // constructs new Block in free space
}

void *hoard::FreeBlockStack::Pop() {
  assert(!empty());
  Block *result = head_;
  head_ = head_->next;
  return reinterpret_cast<void *> (result);
}

bool hoard::FreeBlockStack::empty() {
  return head_ == nullptr;
}

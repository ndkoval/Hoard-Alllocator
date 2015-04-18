#ifndef BLOCK_STACK_H
#define BLOCK_STACK_H

#include "Block.h"

namespace hoard {

  class BlockStack  {


  public:
    BlockStack() : head_(nullptr) {
    }

    BlockStack(const BlockStack &) = delete;
    BlockStack & operator=(const BlockStack &) = delete;

    void Push(Block *t) {
      t->set_next(head_);
      head_ = t;
    }

    Block *Pop() {
      assert(!IsEmpty());
      Block * const t = head_;
      head_ = head_->next();
//    t->set_next(nullptr);
      return t;
    }

    Block * Top() {
      return head_;
    }

    bool IsEmpty() {
      return head_ == nullptr;
    }

    void Reset() {
      head_ = nullptr;
    }

    bool Contains(Block * element) {
      auto * current = head_;
      while (current != nullptr) {
        if (element == current) {
          return true;
        }
        current = current->next();
      }
      return false;
    }


  protected:
    Block *head_;

  };

}
#endif // BLOCK_STACK_H
//
// Created by  Vladimir Skipor on 04/04/15.
//
#include "BaseHeap.h"
#include "Superblock.h"

namespace hoard {
  void BaseHeap::Free(Superblock * superblock, void * ptr) {
    superblock->header().Free(ptr);
    OnFreeSuperblock(superblock);
  }

}



#include "BaseHeap.h"
#include "Superblock.h"

namespace hoard {
  void BaseHeap::Free(Superblock * superblock, void * ptr) {
    superblock->header().Free(ptr);
    OnFreeSuperblock(superblock);
  }
}



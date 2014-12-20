#ifndef GLOBAL_HEAP_H
#define GLOBAL_HEAP_H

#include "BaseHeap.h"

namespace hoard {

template<size_t kSuperblockSize>
class GlobalHeap : public BaseHeap<kSuperblockSize> {
public:
	GlobalHeap() {

	}
};

}


#endif // GLOBAL_HEAP_H

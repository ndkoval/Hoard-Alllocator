#ifndef LOCAL_HEAP_H
#define LOCAL_HEAP_H

#include "BaseHeap.h"

namespace hoard {

template<size_t kSuperblockSize>
class LocalHeap : public BaseHeap<kSuperblockSize> {
public:
	LocalHeap() {

	}
};
}

#endif // LOCAL_HEAP_H

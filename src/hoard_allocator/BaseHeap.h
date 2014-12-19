#ifndef BASE_HEAP_H
#define BASE_HEAP_H

#include <mutex>
#include <hoard_constants.h>

namespace hoard {

class BaseHeap {
public:
	hoard::lock_t lock;

	bool operator==(const BaseHeap& another) {
		return this == &another;
	}
};

}

#endif // BASE_HEAP_H

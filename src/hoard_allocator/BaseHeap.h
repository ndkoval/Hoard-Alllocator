#ifndef BASE_HEAP_H
#define BASE_HEAP_H

#include <mutex>
#include <hoard_constants.h>

namespace hoard {

class BaseHeap {
public:

	bool operator==(const BaseHeap& another) {
		return this == &another;
	}

protected:
	hoard::lock_t lock_;
};

}

#endif // BASE_HEAP_H

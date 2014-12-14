#include <cstdlib>

namespace hoard {
void * SharedMalloc(size_t size) {
    return malloc(size);
}
}

#include "hoard_constants.h"

namespace hoard {
const size_t PAGE_SIZE = sysconf(_SC_PAGESIZE);
const size_t PAGES_IN_SUPERBLOCK = SUPERBLOCK_SIZE / PAGE_SIZE; // power of 2
const size_t NUMBER_OF_CPU = sysconf(_SC_NPROCESSORS_ONLN);
const size_t NUMBER_OF_HEAPS = NUMBER_OF_CPU * 2;
}

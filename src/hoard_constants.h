#ifndef HOARDALLOCATOR_H
#define HOARDALLOCATOR_H
namespace hoard {

    constexpr size_t BLOCK_FACTOR = 2; // power of 2  and > 1
    constexpr size_t MIN_BLOCK_SIZE = 8;  //power of BLOCK_FACTOR
    constexpr size_t EMPTYNESS_FACTOR = 4; // power of 2; 1/EMPTYNESS_FACTOR = f from hoard doc
    size_t PAGE_SIZE =  sysconf(_SC_PAGESIZE);
    constexpr size_t SUPERBLOCK_SIZE = 4 * 1024 * 8; // power of 2
    size_t PAGES_IN_SUPERBLOCK = SUPERBLOCK_SIZE / PAGE_SIZE; // power of 2
    size_t NUMBER_OF_CPU = sysconf( _SC_NPROCESSORS_ONLN );
    size_t NUMBER_OF_HEAPS = NUMBER_OF_CPU * 2;
    constexpr int MAGIC_NUMBER = 0xdeadbeef;
    //static size_t


}


#endif // HOARDALLOCATOR_H

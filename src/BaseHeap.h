#ifndef BASE_HEAP_H
#define BASE_HEAP_H

#include <mutex>

class BaseHeap {
public:
    std::mutex lock;
};

#endif // BASE_HEAP_H

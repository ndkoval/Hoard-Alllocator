#include <memory>

namespace hoard {

template<typename T> class mmap_allocator: public std::allocator<T>
{

};

}

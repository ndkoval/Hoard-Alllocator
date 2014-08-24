#include <memory>
#include <sys/mman.h>

namespace hoard {

template<typename T> class mmap_std_allocator: public std::allocator<T>
{
public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef const T* const_pointer;

    template<typename U>
    struct rebind
    {
        typedef mmap_std_allocator<U> other;
    };

    mmap_std_allocator() {}
    mmap_std_allocator(const mmap_std_allocator &a) {}

    pointer allocate(size_type n, std::allocator<void>::const_pointer hint=0)
    {
        size_t total_size = n * sizeof(T);
        void* ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        return (pointer) ptr;
    }

    void deallocate(pointer p, size_type n)
    {
        if (p == NULL)
            return;
        munmap(p, n * sizeof(T));
    }
};

}

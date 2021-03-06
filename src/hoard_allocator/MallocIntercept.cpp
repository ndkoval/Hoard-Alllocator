#include <cerrno>
#include <utils.h>
#include "hoard_constants.h"
#include "Internals.h"
#include "cstring"

using namespace hoard;

namespace {
__thread bool insideMalloc = false;

struct recursionGuard {
	recursionGuard() {
#ifdef NDEBUG
		if (insideMalloc) {
			print("recursive call\n");
			std::abort();
		}
#endif

		insideMalloc = true;
	}

	~recursionGuard() {
		insideMalloc = false;
	}

private:
	recursionGuard(recursionGuard const &);

	recursionGuard &operator=(recursionGuard const &);
};
}


extern "C"
void *malloc(size_t size) __THROW {
	recursionGuard rg;

	trace("try malloc ", size);
	void *p = InternalAlloc(size);
	trace("malloc ", size, " ", p, "\n");

	return p;
}

extern "C"
void *calloc(size_t n, size_t size) __THROW {
	recursionGuard rg;

	trace("try calloc ", n, " ", size);
	void *p = InternalAlloc(n * size);
  std::memset(p, 0, size * n);

	trace("calloc ", n, " ", size, " ", p, "\n");

	return p;
}

extern "C"
void free(void *ptr) __THROW {
	recursionGuard rg;

	trace("try free ", ptr, "\n");
	InternalFree(ptr);
	trace("free ", ptr, "\n");
}

extern "C"
void *realloc(void *ptr, size_t size) __THROW {
	recursionGuard rg;

	trace("try realloc ", ptr, " ", size);
	void *p = InternalRealloc(ptr, size);
	trace("realloc ", ptr, " ", size, " ", p, "\n");

	return p;
}

extern "C"
int posix_memalign(void **memptr, size_t alignment, size_t size) __THROW {
	recursionGuard rg;

	*memptr = 0;

	if (!IsValidAlignment(alignment))
		return EINVAL;

	void *p = InternalAlloc(size, alignment);

	trace("posix_memalign ", alignment, " ", size, " ", p, "\n");

	if (p == 0)
		return ENOMEM;

	*memptr = p;

	return 0;
}

extern "C"
void *valloc(size_t size) __THROW {
	recursionGuard rg;

	print("deprecated function valloc is not supported\n");
	std::abort();
}

extern "C"
void *memalign(size_t boundary, size_t size) __THROW {
	recursionGuard rg;

	print("deprecated function memalign is not supported\n");
	std::abort();
}

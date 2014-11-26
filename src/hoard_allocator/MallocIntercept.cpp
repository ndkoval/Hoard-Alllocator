#include <cerrno>
#include <utils.h>

#include "Internals.h"
#include "tracing.h"

using namespace hoard;

namespace {
thread_local bool insideMalloc = false;

struct recuirsionGuard {
  recuirsionGuard() {
    if (insideMalloc) {
      print("recursive call\n");
      std::abort();
    }

    insideMalloc = true;
  }

  ~recuirsionGuard() {
    insideMalloc = false;
  }

private:
  recuirsionGuard(recuirsionGuard const &);

  recuirsionGuard &operator=(recuirsionGuard const &);
};
}


extern "C"
void *malloc(size_t size) {
  recuirsionGuard rg;

  void *p = InternalAlloc(size);
  trace("malloc ", size, " ", p, "\n");

  return p;
}

extern "C"
void *calloc(size_t n, size_t size) {
  recuirsionGuard rg;

  void *p = InternalAlloc(n * size);
  trace("calloc ", n, " ", size, " ", p, "\n");

  return p;
}

extern "C"
void free(void *ptr) {
  recuirsionGuard rg;

  InternalFree(ptr);
  trace("free ", ptr, "\n");
}

extern "C"
void *realloc(void *ptr, size_t size) {
  recuirsionGuard rg;

  void *p = InternalRealloc(ptr, size);
  trace("realloc ", ptr, " ", size, " ", p, "\n");

  return p;
}

extern "C"
int posix_memalign(void **memptr, size_t alignment, size_t size) {
  recuirsionGuard rg;

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
void *valloc(size_t size) {
  recuirsionGuard rg;

  print("deprecated function valloc is not supported\n");
  std::abort();
}

extern "C"
void *memalign(size_t boundary, size_t size) {
  recuirsionGuard rg;

  print("deprecated function memalign is not supported\n");
  std::abort();
}

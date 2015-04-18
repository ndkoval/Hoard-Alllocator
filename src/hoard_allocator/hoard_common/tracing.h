#ifndef TRACING_H
#define TRACING_H

#include <cstdlib>
#include <limits>

// Generally its a bad idea to call I/O function from malloc
// if they call malloc we will end up with an infinite recursion.
// This file Contains simple tracing functions which don't use malloc.
// You can use them instead of std::cerr or fprintf.
// This code is known to be incomplete and incorrect and it lacks
// support of signed types.

namespace hoard {
void print_object(char const *);

void print_object(void *px);

void print_object(size_t n);

void print_object(bool b);

void print_object(long long l);

void print_object(int i);

void print();

template<typename T, typename ... Ts>
void print(T obj, Ts ... objs) {
	print_object(obj);
	print(objs...);
}

template<typename ... Ts>
void println(Ts ... objs) {
	print(objs..., "\n");
}

  struct StopTraceGuard {
    StopTraceGuard();
    ~StopTraceGuard();

  private:
    const bool initial_value_;



  };

bool trace_enabled();

template<typename ... Ts>
void trace(Ts ... objs) {
  #ifndef NDEBUG
	if (!trace_enabled())
		return;

	println(objs...);
	#endif
}
template<typename ... Ts>
void fatal_error(Ts ... objs) {
  println("Fatal Error:", objs...);
  println("File: ", __BASE_FILE__, " Line: ", __LINE__);
  std::abort();
}

template<typename ... Ts>
void check_fatal(bool condition, Ts ... objs) {
  if(!condition) {
    fatal_error(objs...);
  }
}

template<typename ... Ts>
void check_debug(bool condition, Ts ... objs) {
#ifndef NDEBUG
  check_fatal(condition, objs...);
#endif
}

}
#endif

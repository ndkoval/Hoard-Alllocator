Hoard Memory Allocator
======================
[![Build Status](https://travis-ci.org/ndkoval/Hoard-Allocator.svg?branch=master)](https://travis-ci.org/ndkoval/Hoard-Allocator)

run (no trace):       LD_PRELOAD=libmalloc_intercept.so MALLOC_INTERCEPT_NO_TRACE=1 kreversi
run (trace):    LD_PRELOAD=libmalloc_intercept.so kreversi

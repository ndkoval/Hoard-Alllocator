Hoard Memory Allocator
======================
[![Build Status](https://travis-ci.org/ndkoval/Hoard-Allocator.svg?branch=master)](https://travis-ci.org/ndkoval/Hoard-Allocator)

run (trace):       LD_PRELOAD=./libHoard-Allocator.so MALLOC_INTERCEPT_NO_TRACE=1 kreversi
run (no trace):    LD_PRELOAD=./libHoard-Allocator.so kreversi

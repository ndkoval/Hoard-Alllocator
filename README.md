Hoard Memory Allocator
======================

run (trace):       LD_PRELOAD=./libHoard-Allocator.so MALLOC_INTERCEPT_NO_TRACE=1 kreversi
run (no trace):    LD_PRELOAD=./libHoard-Allocator.so kreversi

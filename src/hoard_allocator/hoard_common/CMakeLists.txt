add_library(hoard_common
  hoard_constants.h
  utils.h
  tracing.h
  tracing.cpp
  AllocFreeHashMap.h
../mmap_std_allocator.h)
target_include_directories(hoard_common PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(dummy/)
add_subdirectory(hoard_common/)

include_directories(hoard_common/)

add_library(hoard_core
  hoard_common/AllocFreeHashMap.h
  BaseHeap.h
  BaseHeap.cpp
  FreeSuperblockManager.h
  FreeSuperblockManager.cpp
  GlobalHeap.h
  GlobalHeap.cpp
  Internals.h
  Internals.cpp
  LocalHeap.h
  LocalHeap.cpp
  superblock.h
  superblock.cpp
)
target_link_libraries(hoard_core
  hoard_common
)
target_include_directories(hoard_core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(malloc_intercept
  MallocIntercept.cpp
)

target_link_libraries(malloc_intercept
  hoard_core
)

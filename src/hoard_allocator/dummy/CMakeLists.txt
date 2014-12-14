add_library(dummy_malloc_intercept SHARED
  dummy_malloc_intercept.cpp
)
target_link_libraries(dummy_malloc_intercept hoard_common)

add_library(die_malloc_intercept SHARED
  die_malloc_intercept.cpp
)
target_link_libraries(die_malloc_intercept hoard_common)


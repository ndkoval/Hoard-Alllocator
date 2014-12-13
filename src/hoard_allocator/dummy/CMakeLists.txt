add_library(dummy_malloc_intercept
  dummy_malloc_intercept.cpp
)
target_link_libraries(dummy_malloc_intercept hoard_common)


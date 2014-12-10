add_library(hoard_common
  hoard_constants.h
  hoard_constants.cpp
  utils.h
  tracing.h
  tracing.cpp
)
target_include_directories(hoard_common PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR}
)

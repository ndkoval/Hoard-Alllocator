function(add_test_macro name)
  add_executable(${name} ${ARGN} )
  add_test(NAME ${name} COMMAND ${name})
endfunction()

function(add_gtest name)
  add_test_macro(${name} ${ARGN} )
  target_link_libraries(${name} gtest_main)
endfunction()

function(add_noalloc_test name)
  add_test_macro(${name} ${ARGN} )
  target_link_libraries(${name} die_malloc_intercept)
endfunction()

function(add_dummy_alloc_test name)
  add_executable(${name} ${ARGN} )
  get_target_property(VAR dummy_malloc_intercept LOCATION)
  add_test(NAME ${name} COMMAND ${name})
  if(APPLE)
      SET_PROPERTY(TEST ${name} PROPERTY ENVIRONMENT "DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_PRINT_LIBRARIES=1 DYLD_INSERT_LIBRARIES=${VAR}")
      SET_PROPERTY(TEST ${name} PROPERTY ENVIRONMENT "DYLD_INSERT_LIBRARIES=${VAR}")
      set(ENV{DYLD_INSERT_LIBRARIES} "${VAR}" )
  else(APPLE)
      SET_PROPERTY(TEST ${name} PROPERTY ENVIRONMENT "LD_PRELOAD=${VAR}")
  endif(APPLE)


  #target_link_libraries(${name} dummy_malloc_intercept)
  target_link_libraries(${name} hoard_common)
endfunction()

SET (CTEST_ENVIRONMENT
  "DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_PRINT_LIBRARIES=1 DYLD_INSERT_LIBRARIES=${VAR}"
)

  get_target_property(VAR dummy_malloc_intercept LOCATION)
  message(${VAR})
  message("HI!!! $<TARGET_FILE:dummy_malloc_intercept>")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ./../hoard_allocator/hoard_common)

add_gtest(custom_hashmap_test
 custom_hashmap_test.cpp
)
target_link_libraries(custom_hashmap_test
  hoard_common
)

add_noalloc_test(noalloc_testing_test
  noalloc_testing_test.cpp
)

add_noalloc_test(hashmap_noalloc_test
  hashmap_noalloc_test.cpp
)

add_library(shared_malloc SHARED shared_malloc.cpp)
add_dummy_alloc_test(shared_malloc_test shared_malloc_test.cpp)
target_link_libraries(shared_malloc_test shared_malloc)

add_executable(ld_preload_test ld_preload_test.cpp)
target_link_libraries(ld_preload_test hoard_common)


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
  add_test_macro(${name} ${ARGN} )
  target_link_libraries(${name} dummy_malloc_intercept)
endfunction()


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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


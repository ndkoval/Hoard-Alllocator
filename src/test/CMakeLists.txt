#####################################################################
# Helper functions
#####################################################################
function(add_test_macro name)
  add_executable(${name} ${ARGN} )
  add_test(NAME ${name} COMMAND ${name})
  target_link_libraries(${name} hoard_common)
endfunction()

function(set_test_preload test_name lib_name)
  add_dependencies(${test_name} ${lib_name})
  get_target_property(LIB_PATH ${lib_name} LOCATION)
  SET_PROPERTY(TEST ${test_name} PROPERTY ENVIRONMENT "LD_PRELOAD=${LIB_PATH}")
endfunction()

function(add_gtest name)
  add_test_macro(${name} ${ARGN} )
  target_link_libraries(${name} gtest_main)
endfunction()

function(add_noalloc_test name)
  add_test_macro(${name} ${ARGN} )
  set_test_preload(${name} die_malloc_intercept)
endfunction()

function(add_dummy_alloc_test name)
  add_test_macro(${name} ${ARGN})
  set_test_preload(${name} dummy_malloc_intercept)
endfunction()

function(add_hoard_core_gtest name)
  add_gtest(${name} ${ARGN})
  target_link_libraries(${name} hoard_core)
endfunction()

#####################################################################
# Basic setings
#####################################################################
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_target(testlog
  COMMAND cat ./Testing/Temporary/LastTest.log
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_custom_target(check
  COMMAND make all
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_custom_target(kreversi
  COMMAND LD_PRELOAD=./lib/libmalloc_intercept.so kreversi
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_dependencies(kreversi malloc_intercept)

#####################################################################
# Tests
#####################################################################

add_noalloc_test(noalloc_file_preload_test  noalloc_file_preload_test.cpp)

#if dummy is not in LD_PRELOAD, test will crashed
add_dummy_alloc_test(shared_malloc_test shared_malloc_test.cpp)
add_library(shared_malloc SHARED shared_malloc.cpp)
target_link_libraries(shared_malloc_test shared_malloc die_malloc_intercept)

add_dummy_alloc_test(no_alloc_file_preload_test noalloc_file_preload_test.cpp)

add_gtest(custom_hashmap_test custom_hashmap_test.cpp)
add_noalloc_test(noalloc_testing_test noalloc_testing_test.cpp)

add_noalloc_test(hashmap_noalloc_test hashmap_noalloc_test.cpp)

add_gtest(utils_test utils_test.cpp)

add_hoard_core_gtest(global_heap_test global_heap_test.cpp)
add_hoard_core_gtest(big_alloc_test big_alloc_test.cpp)
add_hoard_core_gtest(free_superblock_manager_test free_superblock_manager_test.cpp)
add_hoard_core_gtest(superblock_test superblock_test.cpp)

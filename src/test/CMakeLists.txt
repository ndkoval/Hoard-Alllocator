function(add_gtest name)
  add_executable(${name}
    ${ARGN}
  )
  target_link_libraries(${name} gtest_main)
  add_test(NAME ${name} COMMAND ${name})
endfunction()

function(add_noalloc_test name)
  add_executable(${name}
    ${ARGN}
  )
  target_link_libraries(${name} dummy_malloc_intercept)
  add_test(NAME ${name} COMMAND ${name})
endfunction()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)

add_gtest(custom_hashmap_test
 custom_hashmap_test.cpp
)
target_link_libraries(custom_hashmap_test
  hoard_common
)

add_noalloc_test(single_malloc_test
  single_malloc_test.cpp
)
add_noalloc_test(hashmap_noalloc_test
  hashmap_noalloc_test.cpp
)


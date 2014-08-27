project(Hoard-Allocator)
cmake_minimum_required(VERSION 2.6.4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
set(CMAKE_BUILD_TYPE "Debug")
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
set(SRC_DIR "src")
set(DUMMY_DIR ${SRC_DIR}/dummy)

file(GLOB SRC_LIST ${SRC_DIR}/*.h ${SRC_DIR}/*.cpp)
message(STATUS "SOURCES: ${SRC_LIST}")
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

#Tests
enable_testing()
set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
include_directories(${GTEST_INCLUDE_DIRS} ${SRC_DIR})

#Test mmap_std_allocator
add_executable(mmap_std_allocator_test ${TEST_DIR}/mmap_std_allocator_test.cpp)
add_dependencies(mmap_std_allocator_test googletest)
target_link_libraries(mmap_std_allocator_test ${GTEST_LIBS_DIR}/libgtest.a  ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(mmap_std_allocator_test mmap_std_allocator_test)

#Test no_alloc_hashmap
add_executable(custom_hashmap_test ${TEST_DIR}/custom_hashmap_test.cpp ${SRC_DIR}/tracing.cpp)
add_dependencies(custom_hashmap_test googletest)
target_link_libraries(custom_hashmap_test ${GTEST_LIBS_DIR}/libgtest.a  ${GTEST_LIBS_DIR}/libgtest_main.a pthread)
add_test(custom_hashmap_test custom_hashmap_test)

#Dummy allocator
add_library("dummy_malloc_intercept" SHARED ${DUMMY_DIR}/dummy_malloc_intercept.cpp)

#Test mmap_std_allocator (executable)
add_executable(mmap_std_allocator_executable_test ${TEST_DIR}/mmap_std_allocator_executable_test.cpp)
add_test(mmap_std_allocator_executable_test mmap_std_allocator_executable_test)
set_property(TEST mmap_std_allocator_executable_test PROPERTY WORKING_DIRECTORY .)
set_property(TEST mmap_std_allocator_executable_test PROPERTY ENVIRONMENT "LD_PRELOAD=./libdummy_malloc_intercept.so")

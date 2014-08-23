project(Hoard-Allocator)
cmake_minimum_required(VERSION 2.6.4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
set(CMAKE_BUILD_TYPE "Debug")
set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
set(SRC_DIR "src")

#aux_source_directory (${SRC_DIR} SRC_LIST)
file(GLOB SRC_LIST ${SRC_DIR}/*.h ${SRC_DIR}/*.cpp)
message(STATUS "SOURCES: ${SRC_LIST}")
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

#Tests
set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)
enable_testing()

add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
include_directories(${GTEST_INCLUDE_DIRS} ${SRC_DIR})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)

target_link_libraries(${PROJECT_TEST_NAME}
		${GTEST_LIBS_DIR}/libgtest.a
		${GTEST_LIBS_DIR}/libgtest_main.a
		pthread
	)

add_test(test ${PROJECT_TEST_NAME})


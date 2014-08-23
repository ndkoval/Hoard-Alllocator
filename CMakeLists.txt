project (Hoard-Allocator)
cmake_minimum_required (VERSION 2.6.4)
aux_source_directory (src SRC_LIST)
add_library (${PROJECT_NAME} SHARED ${SRC_LIST})
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
set (CMAKE_BUILD_TYPE "Debug")
set (EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)

#Tests
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
enable_testing()
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)

target_link_libraries(${PROJECT_TEST_NAME}
		${GTEST_LIBS_DIR}/libgtest.a
		${GTEST_LIBS_DIR}/libgtest_main.a
		pthread
	)

add_test(tests ${PROJECT_TEST_NAME})


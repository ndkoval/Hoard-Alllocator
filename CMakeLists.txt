project(Hoard-Allocator)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fPIC -Wall -pedantic")
set(CMAKE_BUILD_TYPE "debug")

enable_testing()
add_subdirectory(src/)



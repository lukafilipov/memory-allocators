# CMake entry point
cmake_minimum_required (VERSION 3.5)
project (memory-allocators)

## Add headers files
include_directories (
	includes/
)


set(SOURCES src/Allocator.cpp src/CAllocator.cpp src/LinearAllocator.cpp src/StackAllocator src/PoolAllocator src/FreeListAllocator.cpp src/Benchmark.cpp src/main.cpp)

add_compile_options(
	-std=c++11
#        -D_DEBUG
#        -g
) 

# main.cpp
add_executable(main ${SOURCES})
target_link_libraries(main)
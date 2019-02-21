cmake_minimum_required(VERSION 3.13)
project(mem_pool)

set(CMAKE_CXX_STANDARD 11)

add_executable(mem_pool main.cpp include/allocator.h include/MemoryPool.h include/MemoryCell.h include/MemoryBlock.h)
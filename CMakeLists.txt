cmake_minimum_required(VERSION 3.22.1)
project(test)

set(CMAKE_CXX_STANDARD 20)

add_executable(test 
    MemoryRiver.hpp
    test_for_memoryriver.cpp
)
cmake_minimum_required(VERSION 3.22.1)
project(test)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall)

add_executable(code 
    src/MemoryRiver.hpp
    src/myhash.hpp
    src/database.hpp
    test_program/test_database.cpp
)
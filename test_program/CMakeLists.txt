cmake_minimum_required(VERSION 3.22.1)
project(test)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall)

add_executable(code 
    ../src/operator.hpp
    ../src/MemoryRiver.hpp
    ../src/Info.hpp
    ../src/Exception.hpp
    ../src/user.hpp
    ../src/mystr.hpp
    ../src/mystack.hpp
    ../src/database.hpp
    ../src/MemoryRiver.hpp
    test_database.cpp
)
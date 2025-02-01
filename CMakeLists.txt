cmake_minimum_required(VERSION 3.22.1)
project(Bookstore-2024)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall)
add_compile_options(-Ofast)

add_executable(code 
    src/operator.hpp
    src/MemoryRiver.hpp
    src/Info.hpp
    src/Exception.hpp
    src/user.hpp
    src/mystr.hpp
    src/mystack.hpp
    src/database.hpp
    src/MemoryRiver.hpp
    main.cpp
)
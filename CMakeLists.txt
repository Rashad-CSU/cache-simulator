cmake_minimum_required(VERSION 3.10)
project(cache_simulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -O2)

add_executable(cache_sim
    src/main.cpp
    src/cache.cpp
)

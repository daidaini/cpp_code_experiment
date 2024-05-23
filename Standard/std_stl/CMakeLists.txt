cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-ggdb")

add_definitions(-DFMT_HEADER_ONLY)

add_executable(test_algorithm test_algorithm.cpp)
#add_executable(test_vector test_vector.cpp)

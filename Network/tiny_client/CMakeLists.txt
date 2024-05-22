cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME "TinyClient")
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") 

add_definitions(-DFMT_HEADER_ONLY)

#include_directories(../../../../CppThirdPart/fmt)

add_executable(${PROJECT_NAME} main.cpp)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32.lib)
endif()


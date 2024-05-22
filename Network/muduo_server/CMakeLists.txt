cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE debug)

macro(add_source source_dir)
    #message("ARGV0 ${ARGV0}")
    include_directories(${ARGV0})
    set(hfind ${ARGV0})
    string(APPEND hfind "/*.h")
    set(cppfind ${ARGV0})
    string(APPEND cppfind "/*.c*")
    file(GLOB name_header ${hfind})
    file(GLOB name_cpp ${cppfind})
    set(all_file ${all_file} ${name_header} ${name_cpp})
    endmacro()

set(PRJ_NAME muduo_server)
project(${PRJ_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -ggdb") 

include_directories(../../ThirdPart)
add_source(../../ThirdPart/muduo/net)
add_source(../../ThirdPart/muduo/base)

add_source(../../common)

add_definitions(-DFMT_HEADER_ONLY)

#message("files = " ${all_file})

add_executable(${PRJ_NAME} ${all_file} main.cpp)

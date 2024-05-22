cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(prjName AsioEcho)
project(${prjName})

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -pipe")

add_definitions(-DFMT_HEADER_ONLY)
add_definitions(-DSPDLOG_FMT_EXTERNAL)
add_definitions(-DSPDLOG_HEADER_ONLY)

#link_directories(/usr/local/lib/boost)

find_library(BOOST_TIMER_LIB libboost_timer.a /usr/local/lib/boost/lib)
find_library(BOOST_THREAD_LIB libboost_thread.a /usr/local/lib/boost/lib)

add_executable(${prjName} main.cc)
target_link_libraries(${prjName} ${BOOST_TIMER_LIB} ${BOOST_THREAD_LIB})

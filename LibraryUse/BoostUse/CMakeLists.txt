cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g") #-ggdb


add_definitions(-DFMT_HEADER_ONLY)

add_executable(test_boost_pool test_boost_pool.cc)

add_executable(test_string_algo test_string_algo.cc)

add_executable(test_boost_date test_boost_date.cc)


# 添加Boost库路径
link_directories(${Boost_LIBRARY_DIRS})

find_library(BOOST_REGEX_LIB libboost_regex.a /usr/local/lib/boost/lib)
message("boost path : " ${BOOST_REGEX_LIB})

add_executable(test_boost_regex test_boost_regex.cc)
target_link_libraries(test_boost_regex ${BOOST_REGEX_LIB})

add_executable(test_string_ref test_string_ref.cc)

find_library(BOOST_FILESYS_LIB libboost_filesystem.a /usr/local/lib/boost/lib)
message("boost path : " ${BOOST_FILESYS_LIB})
add_executable(test_boost_filesys test_boost_filesys.cc)
target_link_libraries(test_boost_filesys ${BOOST_FILESYS_LIB})

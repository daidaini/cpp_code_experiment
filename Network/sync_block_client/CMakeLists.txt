cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

project(sync_block_client)

add_definitions(-DFMT_HEADER_ONLY)

add_executable(sync_block_client main.cpp SyncBlockedClient.cpp)
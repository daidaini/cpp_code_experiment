cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

#set(MYSQL_CONNECTOR_ROOT "/usr/local/mysql/connector-c++-9.1.0")

#include_directories(${MYSQL_CONNECTOR_ROOT}/include)

add_executable(TEST_MYSQL main.cpp)
target_link_libraries(TEST_MYSQL /usr/lib/x86_64-linux-gnu/libmysqlcppconn.so)
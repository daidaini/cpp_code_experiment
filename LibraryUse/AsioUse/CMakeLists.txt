cmake_minimum_required(VERSION 3.10)

project(asio_client)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
if(BOOST_FOUND)
    message(STATUS "Boost version: ${Boost_VERSION}")
    #include_directories(${Boost_INCLUDE_DIRS})
else()
    message(STATUS "Boost coroutine not found")
endif()

add_executable(asio_client main.cpp asio_client.h)

target_link_libraries(asio_client PRIVATE Boost::system)
target_link_directories(asio_client PRIVATE ${Boost_INCLUDE_DIRS})

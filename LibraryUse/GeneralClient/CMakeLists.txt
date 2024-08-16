cmake_minimum_required(VERSION 3.10)

project(GeneralClient)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
if(BOOST_FOUND)
    message(STATUS "Boost version: ${Boost_VERSION}")
else()
    message(STATUS "Boost not found")
endif()

include_directories(../../CommonTools)

add_library(GeneralClient SHARED 
                    ClientImpl.cpp 
                    AsioClient.cpp
                    ../../CommonTools/StringFunc.cc
                    )

target_link_libraries(GeneralClient PRIVATE Boost::system)
target_link_directories(GeneralClient PRIVATE ${Boost_INCLUDE_DIRS})
target_compile_options(GeneralClient PRIVATE -ggdb -pthread)

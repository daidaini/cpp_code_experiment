cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "TinyServer")
project(${PROJECT_NAME})

macro(add_package name dir)
	include_directories(${dir})
	set(hfind ${ARGV1})
	string(APPEND hfind "/*.h")
	set(hppfind ${ARGV1})
	string(APPEND hppfind "/*.hpp")
	set(cppfind ${ARGV1})
	string(APPEND cppfind "/*.cpp")
	set(cfind ${ARGV1})
	string(APPEND cfind "/*.c")
	file(GLOB name_header ${hfind})
	file(GLOB name_hpp ${hppfind})
	file(GLOB name_cpp ${cppfind})
	file(GLOB name_c ${cfind})
	set(all_file ${all_file}  ${name_header}  ${name_hpp} ${name_cpp} ${name_c})
	source_group(${ARGV0} FILES  ${name_header} ${name_hpp} ${name_cpp} ${name_c})
endmacro(add_package)

#add_package(fmt ../../source/fmt/include)

#find_package(fmt CONFIG REQUIRED)
#link_directories(${PROJECT_SOURCE_DIR}/../../source/fmt/lib)

add_executable(${PROJECT_NAME} ${all_file} main.cpp)

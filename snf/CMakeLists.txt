cmake_minimum_required(VERSION 2.8)
project(snf)

set(VERSION "0.1.0")

set(CMAKE_VERBOSE_MAKEFILE ON)

option(BUILD_SHARED_LIBS "build snf shared library" ON)

option(BUILD_STATIC_LIBS "build snf static library" ON)

option(BUILD_EXAMPLE "build example" ON)

set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_FLAGS "-std=c++11 -g -DUSE_SYS_EPOLL")

#set(CMAKE_CXX_FLAGS "-std=c++11 -g")

set(CMAKE_EXE_LINKER_FLAGS "-lpthread -lmysqlclient")

add_subdirectory(src)

if(BUILD_EXAMPLE STREQUAL "ON")
	add_subdirectory(example)
endif()


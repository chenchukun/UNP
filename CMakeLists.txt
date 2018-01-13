cmake_minimum_required(VERSION 3.5)
project(main)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -DDEBUG")

include_directories(com)

set(SOURCE_FILES main.cpp)
add_executable(main ${SOURCE_FILES})
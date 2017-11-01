cmake_minimum_required(VERSION 3.1)
project(UNP)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(daytimetcpsrv chapter_1/daytimetcpsrv.cpp)
add_executable(daytimetcpcli chapter_1/daytimetcpcli.cpp)
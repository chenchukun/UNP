include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(pipe pipe.cpp)
target_link_libraries(pipe snf_static)
target_link_libraries(pipe pthread)

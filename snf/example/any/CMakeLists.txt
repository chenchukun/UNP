include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(any any.cpp)
target_link_libraries(any snf_static)
target_link_libraries(any pthread)

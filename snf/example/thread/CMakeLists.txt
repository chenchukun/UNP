include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(thread thread.cpp)
target_link_libraries(thread snf_static)
target_link_libraries(thread pthread)

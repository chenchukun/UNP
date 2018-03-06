include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(block block.cpp)
target_link_libraries(block snf_static)
target_link_libraries(block pthread)

add_executable(buffer buffer.cpp)
target_link_libraries(buffer snf_static)
target_link_libraries(buffer pthread)

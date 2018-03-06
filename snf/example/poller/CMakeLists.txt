include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(poll poll.cpp)
target_link_libraries(poll snf_static)
target_link_libraries(poll pthread)


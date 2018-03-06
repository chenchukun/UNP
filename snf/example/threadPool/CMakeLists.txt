include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(threadPool threadPool.cpp)
target_link_libraries(threadPool snf_static)
target_link_libraries(threadPool pthread)

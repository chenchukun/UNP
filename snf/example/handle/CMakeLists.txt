include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(ioHandle ioHandle.cpp)
target_link_libraries(ioHandle snf_static)
target_link_libraries(ioHandle pthread)

add_executable(sockHandle sockHandle.cpp)
target_link_libraries(sockHandle snf_static)
target_link_libraries(sockHandle pthread)


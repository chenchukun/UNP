include_directories(${PROJECT_SOURCE_DIR}/include)


add_executable(reactorThread reactorThread.cpp)
target_link_libraries(reactorThread snf_static)
target_link_libraries(reactorThread pthread)

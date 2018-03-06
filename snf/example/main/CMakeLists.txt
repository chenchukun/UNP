include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(main main.cpp)
target_link_libraries(main snf_static)
target_link_libraries(main pthread)

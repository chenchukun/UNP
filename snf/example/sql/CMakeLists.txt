include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(mySql mySql.cpp)
target_link_libraries(mySql snf_static)
target_link_libraries(mySql pthread)

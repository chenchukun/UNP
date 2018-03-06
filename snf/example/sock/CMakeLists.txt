include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(sockStream sockStream.cpp)
target_link_libraries(sockStream snf_static)
target_link_libraries(sockStream pthread)

add_executable(sockConnect sockConnect.cpp)
target_link_libraries(sockConnect snf_static)
target_link_libraries(sockConnect pthread)

add_executable(sockAccept sockAccept.cpp)
target_link_libraries(sockAccept snf_static)
target_link_libraries(sockAccept pthread)

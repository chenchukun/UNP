include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(pingpong_server pingpong_server.cpp)
target_link_libraries(pingpong_server snf_static)
target_link_libraries(pingpong_server pthread)

add_executable(pingpong_client pingpong_client.cpp)
target_link_libraries(pingpong_client snf_static)
target_link_libraries(pingpong_client pthread)

add_executable(send send.cpp)
target_link_libraries(send snf_static)
target_link_libraries(send pthread)

add_executable(echo echo.cpp)
target_link_libraries(echo snf_static)
target_link_libraries(echo pthread)

add_executable(chat chat.cpp)
target_link_libraries(chat snf_static)
target_link_libraries(chat pthread)

add_executable(con con.cpp)
target_link_libraries(con snf_static)
target_link_libraries(con pthread)

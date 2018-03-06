include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(acceptor acceptor.cpp)
target_link_libraries(acceptor snf_static)
target_link_libraries(acceptor pthread)

add_executable(tcpConnection tcpConnection.cpp)
target_link_libraries(tcpConnection snf_static)
target_link_libraries(tcpConnection pthread)

add_executable(tcpServer tcpServer.cpp)
target_link_libraries(tcpServer snf_static)
target_link_libraries(tcpServer pthread)

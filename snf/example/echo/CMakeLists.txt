include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(echoSrv echoSrv.cpp)
target_link_libraries(echoSrv snf_static)
target_link_libraries(echoSrv pthread)

add_executable(echoCli echoCli.cpp)
target_link_libraries(echoCli snf_static)
target_link_libraries(echoCli pthread)

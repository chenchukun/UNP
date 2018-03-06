include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(reactor reactor.cpp)
target_link_libraries(reactor snf_static)
target_link_libraries(reactor pthread)

add_executable(timerHeap timerHeap.cpp)
target_link_libraries(timerHeap snf_static)
target_link_libraries(timerHeap pthread)

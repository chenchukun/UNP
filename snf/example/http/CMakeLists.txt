include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(httpd HttpServer.cpp HttpContext.cpp httpd.cpp)
target_link_libraries(httpd snf_static)
target_link_libraries(httpd pthread)

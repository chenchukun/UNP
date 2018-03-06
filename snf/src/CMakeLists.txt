file(GLOB LIB_SRC_LIST "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/include)

if(BUILD_SHARED_LIBS STREQUAL "ON")
	add_library(snf_shared SHARED ${LIB_SRC_LIST})
	target_link_libraries(snf_shared pthread)
	target_link_libraries(snf_shared mysqlclient)
	set_target_properties(snf_shared PROPERTIES OUTPUT_NAME "snf")
	set_target_properties(snf_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	set_target_properties(snf_shared PROPERTIES VERSION 1.0 SOVERSION 1)
endif()

if(BUILD_STATIC_LIBS STREQUAL "ON")
	add_library(snf_static STATIC ${LIB_SRC_LIST})
	target_link_libraries(snf_static mysqlclient)
	set_target_properties(snf_static PROPERTIES OUTPUT_NAME "snf")
	set_target_properties(snf_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
endif()

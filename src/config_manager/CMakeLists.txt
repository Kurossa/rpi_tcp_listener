project (config_manager)

add_library (config_manager 
	config_manager/config_manager.cpp
	config_manager/config_manager.h)

target_include_directories (config_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (config_manager PRIVATE pugixml utilities app_config)

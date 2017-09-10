cmake_minimum_required (VERSION 3.5)
project (config_manager)

add_library (config_manager 
	config_manager/config_manager.cpp
	config_manager/config_manager.h
	config_manager/config_parser.cpp
	config_manager/config_parser.h)

target_include_directories (config_manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (config_manager PRIVATE app_utilities app_config)

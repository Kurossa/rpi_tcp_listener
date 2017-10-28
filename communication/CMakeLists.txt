cmake_minimum_required (VERSION 3.5)
project (communication)

add_library (communication 
	communication/communication.cpp
	communication/communication.h)

target_include_directories (communication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (communication PRIVATE app_utilities app_config config_manager mp3
	PUBLIC msg_parser time)

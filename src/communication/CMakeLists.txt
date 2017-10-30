project (communication)

add_library (communication 
	communication/communication.cpp
	communication/communication.h)

target_include_directories (communication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (communication PRIVATE app_utilities 
	PUBLIC mp3 msg_parser config_manager time)

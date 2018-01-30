project (communication)

add_library (communication 
	communication/communication.cpp
	communication/communication.h
	communication/msg_parser.cpp
	communication/msg_parser.h)

target_include_directories (communication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (communication
	PUBLIC mp3
	config_manager)

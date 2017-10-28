cmake_minimum_required (VERSION 3.5)
project (msg_parser)

add_library (msg_parser 
	msg_parser/msg_parser.cpp
	msg_parser/msg_parser.h
)

target_include_directories (msg_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (msg_parser PRIVATE app_utilities)

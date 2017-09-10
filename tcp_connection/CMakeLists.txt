cmake_minimum_required (VERSION 3.5)
project (tcp_connection)

add_library (tcp_connection 
	tcp_connection/tcp_connection.cpp
	tcp_connection/tcp_connection.h
)

target_include_directories (tcp_connection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (tcp_connection PRIVATE app_utilities)

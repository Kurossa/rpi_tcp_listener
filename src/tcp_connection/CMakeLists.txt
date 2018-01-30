project (tcp_connection)

add_library (tcp_connection 
	tcp_connection/tcp_connection.cpp
	tcp_connection/tcp_connection.h
	tcp_connection/tcp_server.cpp
	tcp_connection/tcp_server.h
	tcp_connection/tcp_client.cpp
	tcp_connection/tcp_client.h
)

target_include_directories (tcp_connection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (tcp_connection PRIVATE utilities)


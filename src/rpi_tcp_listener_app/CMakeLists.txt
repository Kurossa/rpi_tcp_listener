project (rpi_tcp_listener)

add_executable(rpi_tcp_listener rpi_tcp_listener.cpp)

target_link_libraries (rpi_tcp_listener app_utilities zip mp3 app_config tcp_connection communication config_manager)
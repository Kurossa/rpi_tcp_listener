cmake_minimum_required (VERSION 3.5)
project (rpi_tcp_listener)

#find_library(PTHREAD_LIB pthread)
#find_library(AO_LIB ao)
#find_library(MPG123_LIB mpg123)

add_subdirectory(app_config)
add_subdirectory(utilities)
add_subdirectory(zip)
add_subdirectory(time)
add_subdirectory(msg_parser)
add_subdirectory(tcp_connection)
add_subdirectory(config_manager)
add_subdirectory(mp3)
add_subdirectory(communication)

add_executable(rpi_tcp_listener main.cpp)

target_link_libraries (rpi_tcp_listener app_utilities zip mp3 app_config tcp_connection communication config_manager)
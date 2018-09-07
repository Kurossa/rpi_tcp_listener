cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(rpi_tcp_listener_project VERSION 3.0 LANGUAGES CXX)

add_definitions(-g -o0 -Wall -Wextra -Werror -pedantic -std=gnu++11)

add_subdirectory(app_rpi_tcp_listener)
add_subdirectory(app_config)
add_subdirectory(communication)
add_subdirectory(config_manager)
add_subdirectory(mp3)
add_subdirectory(ext/pugixml-1.9)
add_subdirectory(tcp_connection)
add_subdirectory(utilities)

# Unit tests section
# Option -DTEST=ON to enable testing
option(TEST "Build all tests" OFF)

if (TEST)
	message (STATUS "Unit tests enabled")
	include (testing.cmake)
else ()
	message (STATUS "Unit tests disabled")
endif()

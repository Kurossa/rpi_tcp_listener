project (time)

add_library (time 
	time/time.cpp
	time/time.h)

target_include_directories (time PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (time PRIVATE app_utilities)

# Unit tests section
include (testing.cmake)

project (zip)

add_library (zip 
	zip/zip.cpp
	zip/zip.h
)

target_include_directories (zip PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (zip PRIVATE app_utilities)

# Unit tests section
include (testing.cmake)

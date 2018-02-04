project (utilities)

# Create a library called "utilites" which includes the source files.
add_library (utilities
	utilities/logger.cpp
	utilities/logger.h
	utilities/time.cpp
	utilities/time.h
	utilities/message_queue.h
	utilities/zip.cpp
	utilities/zip.h
)

target_include_directories (utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (utilities)

project (app_utilities)

find_library(PTHREAD_LIB pthread)

# Create a library called "utilites" which includes the source files.
add_library (app_utilities
	utilities/file_handler.cpp
	utilities/file_handler.h 
	utilities/logger.cpp
	utilities/logger.h
	utilities/message_queue.h
	utilities/thread_wrapper.h
)

target_include_directories (app_utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (app_utilities PRIVATE "${PTHREAD_LIB}")

cmake_minimum_required (VERSION 3.5)
project (app_utilities)

find_library(PTHREAD_LIB pthread)

# Create a library called "utilites" which includes the source files.
# The extension is already found. Any number of sources could be listed here.
add_library (app_utilities
	utilities/file_handler.cpp
	utilities/file_handler.h 
	utilities/logger.cpp
	utilities/logger.h
	utilities/message_queue.h
	utilities/thread_wrapper.h
)
#  logger.cpp  message_queue.hpp file_handler.hpp  logger.hpp  thread_wrapper.hpp

# Make sure the compiler can find include files for our utilities library
# when other libraries or executables link to Hello
target_include_directories (app_utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (app_utilities PRIVATE "${PTHREAD_LIB}")

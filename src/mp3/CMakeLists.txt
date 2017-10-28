cmake_minimum_required (VERSION 3.5)
project (mp3)

find_library(PTHREAD_LIB pthread)
find_library(AO_LIB ao)
find_library(MPG123_LIB mpg123)

add_library (mp3 
	mp3/mp3.cpp
	mp3/mp3.h
)

target_include_directories (mp3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (mp3 PRIVATE "${PTHREAD_LIB}" "${MPG123_LIB}" "${AO_LIB}"
	PUBLIC app_utilities app_config)


include_directories(${gtest_SOURCE_DIR}/include)
add_executable(mp3_player_test
	mp3_player_test.cpp)

target_link_libraries(mp3_player_test
	mp3
	utilities
	gtest
	gtest_main)

add_test(mp3_player_test mp3_player_test)

configure_file(${PROJECT_SOURCE_DIR}/../assets/123.mp3 ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)


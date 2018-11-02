add_executable(communication_test
	communication_test.cpp)

target_link_libraries(communication_test
	PRIVATE communication
	gmock
	gtest_main)

add_test(communication_test communication_test)

configure_file(${PROJECT_SOURCE_DIR}/../assets/audio_app_cfg.xml ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/../assets/123.mp3 ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

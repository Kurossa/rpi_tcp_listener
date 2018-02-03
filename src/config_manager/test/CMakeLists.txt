
add_executable(pugixml_test
	pugixml_test.cpp)

target_link_libraries(pugixml_test
	PRIVATE pugixml
	gmock
	gtest_main)

add_test(pugixml_test pugixml_test)

configure_file(${PROJECT_SOURCE_DIR}/../assets/audio_app_cfg.xml ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)


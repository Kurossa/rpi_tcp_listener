add_executable(msg_parser_test
	msg_parser_test.cpp)

target_link_libraries(msg_parser_test
	PRIVATE communication
	gmock
	gtest_main)

add_test(msg_parser_test msg_parser_test)

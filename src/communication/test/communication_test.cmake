add_executable(communication_test
	communication_test.cpp)

target_link_libraries(communication_test
	PRIVATE communication
	gmock
	gtest_main)

add_test(communication_test communication_test)

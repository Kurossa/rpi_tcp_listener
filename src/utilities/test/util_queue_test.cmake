add_executable(util_queue_test
	util_queue_test.cpp)

target_link_libraries(util_queue_test
	utilities
	gmock
	gtest_main)

add_test(util_queue_test util_queue_test)

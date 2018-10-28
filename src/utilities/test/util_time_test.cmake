add_executable(util_time_test
    mock_util_time.cpp	
    util_time_test.cpp)

target_link_libraries(util_time_test
	utilities
	gtest
	gtest_main)

add_test(util_time_test util_time_test)


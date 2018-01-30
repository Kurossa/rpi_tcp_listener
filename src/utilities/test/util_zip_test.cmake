add_executable(util_zip_test
	util_zip_test.cpp)

target_link_libraries(util_zip_test
	utilities
	gtest
	gtest_main)

add_test(util_zip_test util_zip_test)

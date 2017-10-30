project (zip)

add_library (zip 
	zip/zip.cpp
	zip/zip.h
)

target_include_directories (zip PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (zip PRIVATE app_utilities)

# Unit tests
if (TEST)
	include_directories(${gtest_SOURCE_DIR}/include)
	add_executable(zip_test
		test/zip_test.cpp)

	target_link_libraries(zip_test 
		zip
		gtest
		gtest_main)

	add_test(zip_test zip_test)
endif()

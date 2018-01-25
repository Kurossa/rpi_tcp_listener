add_executable(zip_test
    zip_test.cpp)

target_link_libraries(zip_test
    utilities
    zip
    gtest
    gtest_main)

add_test(zip_test zip_test)



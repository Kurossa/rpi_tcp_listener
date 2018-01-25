add_executable(util_queue_test
    util_queue_test.cpp)

target_link_libraries(util_queue_test
    utilities
    #gtest
    gmock
    gtest_main)

add_test(util_queue_test util_queue_test)

add_executable(util_time_test
    util_time_test.cpp)

target_link_libraries(util_time_test
    utilities
    gtest
    gtest_main)

add_test(util_time_test util_time_test)


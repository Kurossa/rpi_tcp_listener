include_directories(${gtest_SOURCE_DIR}/include)
add_executable(tcp_connection_test
    tcp_connection_test.cpp)

target_link_libraries(tcp_connection_test
    app_utilities
    tcp_connection
    gtest
    gtest_main)

add_test(tcp_connection_test tcp_connection_test)


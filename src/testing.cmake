enable_testing()
add_subdirectory(ext/gtest-1.7.0)
include_directories(${gtest_SOURCE_DIR}/include)

add_subdirectory(zip/test)
add_subdirectory(time/test)
add_subdirectory(tcp_connection/test)
add_subdirectory(utilities/test)


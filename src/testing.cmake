include(CTest)
enable_testing()

add_subdirectory(ext/googletest-1.7.0/gmock)
include_directories(${gtest_SOURCE_DIR}/include
					${gmock_SOURCE_DIR}/include)

add_subdirectory(config_manager/test)
add_subdirectory(mp3/test)
add_subdirectory(tcp_connection/test)
add_subdirectory(utilities/test)


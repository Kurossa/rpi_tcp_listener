if (TEST)
    include_directories(${gtest_SOURCE_DIR}/include)
    add_executable(time_test
        test/time_test.cpp)

    target_link_libraries(time_test
            app_utilities
            time
            gtest
            gtest_main)

    add_test(time_test time_test)
endif()

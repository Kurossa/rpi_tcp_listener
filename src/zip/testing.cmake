if (TEST)
    include_directories(${gtest_SOURCE_DIR}/include)
    add_executable(zip_test
        test/zip_test.cpp)

    target_link_libraries(zip_test
        app_utilities
        zip
        gtest
        gtest_main)

    add_test(zip_test zip_test)
endif()

find_package(GTest REQUIRED)

add_executable(run_tests
    ../${WEB_PAGE_DIR_PATH}/source.cpp
    tests_lru_cache_corr.cpp
    tests_lru2q_cache_corr.cpp
    tests_proph_cache_corr.cpp
)

add_executable(check_cache_eff
    ../${WEB_PAGE_DIR_PATH}/source.cpp
    test_lru2q_cache_eff.cpp
)

target_compile_options(run_tests       PRIVATE -DNO_LOG -DTESTS_)
target_compile_options(check_cache_eff PRIVATE -DTESTS_)
set_target_properties(run_tests       PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
set_target_properties(check_cache_eff PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

target_link_libraries(run_tests
    GTest::gtest
    GTest::gtest_main
    my_loglib
)

target_link_libraries(check_cache_eff PRIVATE my_loglib)

add_test(NAME RunAllTests COMMAND run_tests)

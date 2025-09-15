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

set(CMAKE_CXX_FLAGS  "-D NO_LOG -D TESTS_ ${CMAKE_CXX_FLAGS}")
set_target_properties(run_tests       PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
set_target_properties(check_cache_eff PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

find_library(MY_LOG_LIB ${MY_LOG_LIB_NAME})

target_link_libraries(run_tests
    GTest::gtest
    GTest::gtest_main
    ${MY_LOG_LIB}
)

target_link_libraries(check_cache_eff PRIVATE ${MY_LOG_LIB})

add_test(NAME RunAllTests COMMAND run_tests)

find_package(GTest REQUIRED)

add_executable(run_tests
  check_corr/tests_lru_cache_corr.cpp
  check_corr/tests_lru2q_cache_corr.cpp
  check_corr/tests_proph_cache_corr.cpp
)

target_compile_options(    run_tests PRIVATE ${COMPILATION_FLAGS})
target_compile_definitions(run_tests PRIVATE NO_LOG TESTS_)
set_target_properties(     run_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
target_include_directories(run_tests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/
  ${CMAKE_CURRENT_SOURCE_DIR}/tests_configs/
)

target_link_libraries(run_tests
  GTest::gtest
  GTest::gtest_main
  my_loglib
  my_project_includes
)

add_test(NAME RunAllTests COMMAND run_tests)

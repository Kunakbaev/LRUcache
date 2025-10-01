add_executable(test_perf_lru_cache      plain_task.cpp)
add_executable(test_perf_2q_cache       plain_task.cpp)
add_executable(test_perf_prophecy_cache plain_task.cpp)

set_target_properties(test_perf_lru_cache      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
set_target_properties(test_perf_2q_cache       PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
set_target_properties(test_perf_prophecy_cache PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

target_compile_options(test_perf_lru_cache      PRIVATE -DTEST_PERF_ -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_LRU_CACHE_)
target_compile_options(test_perf_2q_cache       PRIVATE -DTEST_PERF_ -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_2Q_CACHE_)
target_compile_options(test_perf_prophecy_cache PRIVATE -DTEST_PERF_ -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_PROPHECY_CACHE_)

target_link_libraries(test_perf_lru_cache      PRIVATE my_loglib)
target_link_libraries(test_perf_2q_cache       PRIVATE my_loglib)
target_link_libraries(test_perf_prophecy_cache PRIVATE my_loglib)

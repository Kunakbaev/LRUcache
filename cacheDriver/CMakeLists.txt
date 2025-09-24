add_executable(lru_cache
  main.cpp
  ../${WEB_PAGE_DIR_PATH}/source.cpp
)

add_executable(2q_cache
  main.cpp
  ../${WEB_PAGE_DIR_PATH}/source.cpp
)

add_executable(prophecy_cache
  main.cpp
  ../${WEB_PAGE_DIR_PATH}/source.cpp
)

add_executable(test_perf
  plain_task.cpp
  ../${WEB_PAGE_DIR_PATH}/source.cpp
)

set_target_properties(lru_cache      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
set_target_properties(2q_cache       PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
set_target_properties(prophecy_cache PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
set_target_properties(test_perf      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
target_compile_options(test_perf      PRIVATE -DDONT_WANT_SLOW_GET_PAGE_ -DPERF_TEST_)
target_compile_options(lru_cache      PRIVATE -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_LRU_CACHE_)
target_compile_options(2q_cache       PRIVATE -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_2Q_CACHE_)
target_compile_options(prophecy_cache PRIVATE -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_PROPHECY_CACHE_)

# find_library(MY_LOG_LIB ${MY_LOG_LIB_NAME})
target_link_libraries(lru_cache      PRIVATE my_loglib)
target_link_libraries(2q_cache       PRIVATE my_loglib)
target_link_libraries(prophecy_cache PRIVATE my_loglib)

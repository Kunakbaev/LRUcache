add_executable(cache_usecase
  main.cpp
  ../${WEB_PAGE_DIR_PATH}/source.cpp
)

add_executable(test_perf
  plain_task.cpp
  ../${WEB_PAGE_DIR_PATH}/source.cpp
)

set_target_properties(cache_usecase PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
set_target_properties(test_perf     PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
target_compile_options(test_perf PRIVATE -DPERF_TEST_)

find_library(MY_LOG_LIB ${MY_LOG_LIB_NAME})
target_link_libraries(cache_usecase PRIVATE ${MY_LOG_LIB})

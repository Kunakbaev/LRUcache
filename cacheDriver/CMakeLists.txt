add_executable(cache_usecase
  main.cpp
  ../${WEB_PAGE_DIR_PATH}/source.cpp
)

set_target_properties(cache_usecase PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

find_library(MY_LOG_LIB ${MY_LOG_LIB_NAME})
target_link_libraries(cache_usecase PRIVATE ${MY_LOG_LIB})

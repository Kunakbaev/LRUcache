# add_executable(lru_cache      main.cpp)
# add_executable(2q_cache       main.cpp)
# add_executable(prophecy_cache main.cpp)

# set_target_properties(lru_cache      PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
# set_target_properties(2q_cache       PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
# set_target_properties(prophecy_cache PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")

# target_compile_options(lru_cache      PRIVATE -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_LRU_CACHE_)
# target_compile_options(2q_cache       PRIVATE -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_2Q_CACHE_)
# target_compile_options(prophecy_cache PRIVATE -DDONT_WANT_SLOW_GET_PAGE_ -DRUN_PROPHECY_CACHE_)

# target_link_libraries(lru_cache      PRIVATE my_loglib)
# target_link_libraries(2q_cache       PRIVATE my_loglib)
# target_link_libraries(prophecy_cache PRIVATE my_loglib)

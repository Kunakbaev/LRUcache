add_executable(check_cache_eff
  test_lru2q_cache_eff.cpp
)

target_compile_options(check_cache_eff PRIVATE -DTESTS_)
set_target_properties( check_cache_eff PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
target_link_libraries( check_cache_eff PRIVATE my_loglib my_project_includes)

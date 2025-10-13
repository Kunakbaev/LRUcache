function(create_cache_usecase_inp_prompts_target target_name source_file cache_type)
  add_executable(        ${target_name} ${source_file})
  set_target_properties( ${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR_PATH}")
  # target_compile_options(${target_name} PRIVATE ${COMPILATION_FLAGS})
  
  if(cache_type STREQUAL "LRU")
    target_compile_definitions(${target_name} PRIVATE DONT_WANT_SLOW_GET_PAGE_ RUN_LRU_CACHE_)
  elseif(cache_type STREQUAL "2Q")
    target_compile_definitions(${target_name} PRIVATE DONT_WANT_SLOW_GET_PAGE_ RUN_2Q_CACHE_)
  elseif(cache_type STREQUAL "PROPHECY")
    target_compile_definitions(${target_name} PRIVATE DONT_WANT_SLOW_GET_PAGE_ RUN_PROPHECY_CACHE_)
  endif()
  
  target_link_libraries(${target_name} PRIVATE my_loglib my_project_includes)
endfunction()

create_cache_usecase_inp_prompts_target(with_inp_val_lru_cache      with_input_validation.cpp "LRU")
create_cache_usecase_inp_prompts_target(with_inp_val_2q_cache       with_input_validation.cpp "2Q") 
create_cache_usecase_inp_prompts_target(with_inp_val_prophecy_cache with_input_validation.cpp "PROPHECY")

cmake_minimum_required(VERSION 3.11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_C_STANDARD   20)

project("LRUcache")

# --------------    set compilation flags   ----------------

# basic flags for compilation and debugging
set(DEBUG_FLAGS -ggdb3 -fno-omit-frame-pointer)

# general warnings
set(WARNING_FLAGS -Wall -Wextra -Wpedantic)

# security flags
set(SECURITY_FLAGS -fstack-protector -pie -fPIE -Wformat=2 -Wformat-security)

# specific flags
set(CPP_SPECIFIC_FLAGS -Wnon-virtual-dtor -Woverloaded-virtual)

set(COMMON_CXX_FLAGS ${WARNING_FLAGS})

# ------------    specifying compilation options based on build type    ------------

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(NO_LOG NDEBUG)
  set(COMPILATION_FLAGS ${COMMON_CXX_FLAGS} -O2 -DNDEBUG)
  message(STATUS "Building in Release mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(_DEBUG)
  set(COMPILATION_FLAGS ${COMMON_CXX_FLAGS} -O0 -g ${SECURITY_FLAGS} ${CPP_SPECIFIC_FLAGS})
  message(STATUS "Building in Debug mode")
endif()

# --------------------------------------------------------

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND USE_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined)
  add_link_options(-fsanitize=address,undefined)
  message(STATUS "Running with sanitizers")
endif()

# ----------------    adding logger lib submodule   -------------------

# 1. Сначала генерируем config файл
include(CMakePackageConfigHelpers)
configure_package_config_file(
    3dPartyModules/LoggerLib/cmake/my_loglibConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/my_loglibTargets.cmake
    INSTALL_DESTINATION lib/cmake/my_loglib
)

# 2. Указываем КОНКРЕТНЫЙ путь к файлу
set(my_loglib_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# 3. Добавляем subdirectory
add_subdirectory(3dPartyModules/LoggerLib)

# ----------------------    show help message target    ------------

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/help_message.txt CONTENT "
Available targets:
  * lru_cache       -> run page requests queries on lru cache
  * 2q_cache        -> run page requests queries on 2q cache
  * prophecy_cache  -> run page requests queries on prophecy cache
  * test_perf_lru_cache, test_perf_2q_cache, test_perf_prophecy_cache -> same targets as 3 previous ones, but without any additional input prompts or message
  * run_tests       -> check correctness of each cache type by running google tests
  * test_perf       -> same as first 3 targets, but only for 2q cache and without any additional input prompts
  * check_cache_eff -> compare hit performance of 3 cache types: lru, 2q and prophecy, show hit performance table

usage examples:
  * to set build type:       cmake -S . -B bin -DCMAKE_BUILD_TYPE=Debug
  * to run lru_cache target: cmake --build bin --target lru_cache
")

add_custom_target(show_help_msg
  COMMAND ${CMAKE_COMMAND} -E cat ${CMAKE_CURRENT_BINARY_DIR}/help_message.txt
)

# --------------------------------------------------------

# Create an interface library
add_library(my_project_includes INTERFACE)

# Add all include directories to the interface
target_include_directories(my_project_includes INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/3dPartyModules/LoggerLib/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/webPage/
  ${CMAKE_CURRENT_SOURCE_DIR}/cacheImplementations/LRUcache/
  ${CMAKE_CURRENT_SOURCE_DIR}/cacheImplementations/2qCache/
  ${CMAKE_CURRENT_SOURCE_DIR}/cacheImplementations/prophecyCache/
  ${CMAKE_CURRENT_SOURCE_DIR}/common/
)

# -----------------------------------------------------

add_subdirectory(usecase/usecaseForTests)
add_subdirectory(usecase/withInputValidation)
add_subdirectory(tests)
add_subdirectory(compare_hit_perf)

# -----------------------------------------------------




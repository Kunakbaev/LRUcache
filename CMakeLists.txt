cmake_minimum_required(VERSION 3.11)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD   17)

set(ENV{CC}  "gcc")
set(ENV{CXX} "g++")

project("LRUcache")

set(COMMON_CXX_FLAGS "-lm -ggdb3 -std=c++17 -Werror -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla")

set(BUILD_DIR_PATH    "${CMAKE_BINARY_DIR}")
set(WEB_PAGE_DIR_PATH "webPage/source/")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # removes assert checks and log lib output
  add_compile_options(-Ofast -DNDEBUG -DNO_LOG)
  message(STATUS "Building in Release mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(${COMMON_CXX_FLAGS})
  add_compile_options(-g -O0 -D_DEBUG)
  message(STATUS "Building in Debug mode")
endif()




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

include_directories(3dPartyModules/LoggerLib/include/)
include_directories(webPage/include/)
include_directories(cacheRealizations/LRUcache/)
include_directories(cacheRealizations/2qCache/)
include_directories(cacheRealizations/prophecyCache/)
include_directories(common/)



# add_custom_target(install_loger_local
#   COMMAND ${CMAKE_MAKE_PROGRAM} install DESTDIR=${CMAKE_BINARY_DIR}/local_install -C ${CMAKE_CURRENT_SOURCE_DIR}
# )

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

add_subdirectory(usecase/usecaseForTests)
add_subdirectory(usecase/cacheDriver)
add_subdirectory(tests)
add_subdirectory(compare_hit_perf)

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
set(MY_LOG_LIB_NAME   "my_loglib")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # removes assert checks and log lib output
  add_compile_options(-Ofast -DNDEBUG -DNO_LOG)
  message(STATUS "Building in Release mode")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-g -O0 -D_DEBUG)
  message(STATUS "Building in Debug mode")
endif()

add_subdirectory(cacheDriver)
add_subdirectory(tests)

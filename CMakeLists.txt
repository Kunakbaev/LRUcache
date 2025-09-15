cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD   17)

set(ENV{CC}  "gcc")
set(ENV{CXX} "g++")

project("LRUcache")

set(COMMON_CXX_FLAGS "-D _DEBUG -lm -ggdb3 -std=c++17 -O0 -Werror -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla")

set(CMAKE_CXX_FLAGS "${COMMON_CXX_FLAGS}")

set(BUILD_DIR_PATH    "${CMAKE_BINARY_DIR}")
set(WEB_PAGE_DIR_PATH "webPage/source/")
set(MY_LOG_LIB_NAME   "my_loglib")

add_subdirectory(cacheDriver)
add_subdirectory(tests)

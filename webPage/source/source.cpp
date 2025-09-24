#include <iostream>
#include <chrono>
#include <thread>
#include <array>

#include "../include/webPageStruct.hpp"

#ifdef _DEBUG
// we don't want to spend a lot of time during debug
static const size_t PAGE_LOAD_TIME = 10;
#else
static const size_t PAGE_LOAD_TIME = 1000;
#endif

static const std::chrono::milliseconds TIMESPAN(PAGE_LOAD_TIME);

// very big array of all pages on the internet
const page_t all_pages[] = {
  page_t(1024, "i am web page",  1),
  page_t(1024, "i am web page2", 2),
  page_t(1024, "i am web page2", 3)
};
const size_t TOT_NUM_OF_PAGES = sizeof(all_pages) / sizeof(*all_pages);

void slow_get_page(std::size_t search_index, page_t& page) {
#ifndef TESTS_
#ifndef PERF_TEST_
#ifndef DONT_WANT_SLOW_GET_PAGE_
  std::cerr << "Your page is loading..." << std::endl;
  std::this_thread::sleep_for(TIMESPAN);
#endif
#endif
#endif

  page.index = search_index;
  // some page search logic
  // for (auto tmp : all_pages) {
  //   if (tmp.index == search_index) {
  //     page = tmp;
  //   }
  // }
}

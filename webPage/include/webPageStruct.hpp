#pragma once

#include <iostream>
#include <string>

class page_t {
 public:
  page_t() = default;

  page_t(std::size_t size, const std::string& data, std::size_t index2)
    : size_(size), data_(data), index(index2) {}

  friend void slow_get_page(std::size_t search_index, page_t& page);

 public:
  std::size_t index = 0;

 private:
  std::size_t size_  = 0;
  std::string data_  = "";
};

inline void slow_get_page(std::size_t search_index, page_t& page) {
#ifndef TESTS_
#ifndef PERF_TEST_
#ifndef DONT_WANT_SLOW_GET_PAGE_
  std::cerr << "Your page is loading..." << "\n";
  std::this_thread::sleep_for(TIMESPAN);
#endif
#endif
#endif

  page.index = search_index;
}

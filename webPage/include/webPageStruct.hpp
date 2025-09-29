#pragma once

#include <iostream>
#include <string>

const std::size_t MAX_PAGE_INDEX = 1024;

class page_t {
 private:
  std::size_t size_  = 0;    // page size
  std::string data_  = "";   // page data

 public:
  std::size_t index = 0;     // page index: 1, 2, ... n

  page_t() = default;
  page_t(std::size_t size, const std::string& data, std::size_t index2)
    : size_(size), data_(data), index(index2) {}
};

void slow_get_page(std::size_t index, page_t& page);

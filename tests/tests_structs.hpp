#pragma once

#include <iostream>

#include "webPageStruct.hpp"

namespace check_correctness {

using test_queries_t = const std::vector<std::pair<bool, std::size_t>>;

struct CacheTestConf {
  std::string    test_name;
  std::size_t    cache_size;
  test_queries_t queries;
};

// testing namespace comes from gtest
class CacheTest : public testing::TestWithParam<CacheTestConf> {
 protected:
  webpage::page_t page;
};

};

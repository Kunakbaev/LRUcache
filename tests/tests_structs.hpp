#pragma once

#include <iostream>

template<std::size_t CacheSize>
class CacheTest : public testing::TestWithParam<std::pair<bool, std::size_t>> {};

using test_queries_t = const std::vector<std::pair<bool, std::size_t>>;

struct CacheTestConf {
  std::string    test_name;
  std::size_t    cache_size;
  test_queries_t queries;
};

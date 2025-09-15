#include <gtest/gtest.h>

#include "tests_structs.hpp"
#include "../LRUcache/include/LRUcacheStruct.hpp"

TEST(CacheTest, TestFromPresentation) {
  page_t page; // we are not using it here
  cache_t<page_t, size_t> cache(4);
  std::vector<std::pair<bool, size_t>> queries = {
    {false, 1},
    {false, 2},
    {false, 3},
    {false, 4},
    {true,  1},
    {true,  2},
    {false, 5},
    {true,  1},
    {true,  2},
    {true,  4},
    {false, 3},
    {true,  4}
  };
  
  for (auto [is_hit, index] : queries) {
    EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);
  }
}

TEST(CacheTest, CacheSize1) {
  page_t page; // we are not using it here
  cache_t<page_t, size_t> cache(1);
  std::vector<std::pair<bool, size_t>> queries = {
    {false, 1},
    {false, 2},
    {false, 3},
    {false, 1},
    {false, 2},
    {false, 3},
    {true,  3},
    {true,  3},
    {false, 1}
  };
  
  for (auto [is_hit, index] : queries) {
    EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);
  }
}

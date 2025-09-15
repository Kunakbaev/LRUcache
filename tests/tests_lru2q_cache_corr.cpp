#include <gtest/gtest.h>

#include "tests_structs.hpp"
#include "../2qCache/2qCache.hpp"

using queries_list_t = std::vector<std::pair<bool, size_t>>;

TEST(CacheTest, TestFromPresentationLRU2qCache) {
  page_t page; // we are not using it here
  std::vector<std::pair<bool, size_t>> queries = {
    {false, 1},
    {false, 2},
    {false, 3},
    {false, 4},
    {false, 1},
    {false, 2},
    {false, 5},
    {true,  1},
    {true,  2},
    {false, 4},
    {false, 3},
    {true,  4}
  };
  lru2q_cache_t<page_t, size_t> cache(4);
  
  setLoggingLevel(DEBUG);
  for (auto [is_hit, index] : queries) {
    LOG_DEBUG_VARS(is_hit, index);
    EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);
    // cache.dump_cache();
  }
}

TEST(CacheTest, CacheSize1LRU2qCache) {
  page_t page; // we are not using it here
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
  lru2q_cache_t<page_t, size_t> cache(1);

  for (auto [is_hit, index] : queries) {
    EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);
  }
}

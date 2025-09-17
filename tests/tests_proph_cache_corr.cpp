#include <gtest/gtest.h>

#include "tests_structs.hpp"
#include "../prophecyCache/include/prophecyCache.hpp"

using queries_list_t = std::vector<std::pair<bool, size_t>>;

static std::vector<size_t> GetRequestsFromQueries(const queries_list_t& queries) {
  std::vector<size_t> reqs = {};
  reqs.reserve(queries.size());
  for (auto& [_, ind] : queries) {
    reqs.emplace_back(ind);
  }

  return reqs;
}

TEST(CacheTest, TestFromPresentationProphCache) {
  page_t page; // we are not using it here
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
    {true,  3},
    {true,  4}
  };
  prophecy_cache_t<page_t, size_t> cache(4, GetRequestsFromQueries(queries));
  
  for (auto [is_hit, index] : queries) {
    EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);
  }
}

TEST(CacheTest, CacheSize1ProphCache) {
  page_t page; // we are not using it here
  std::vector<std::pair<bool, size_t>> queries = {
    {false, 1},
    {false, 2},
    {false, 3},
    {true,  1},
    {false, 2},
    {false, 3},
    {true,  3},
    {true,  3},
    {false, 1}
  };
  prophecy_cache_t<page_t, size_t> cache(1, GetRequestsFromQueries(queries));

  for (auto [is_hit, index] : queries) {
    EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);
  }
}

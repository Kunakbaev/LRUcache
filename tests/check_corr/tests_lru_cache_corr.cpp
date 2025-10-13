#include <gtest/gtest.h>

#include "tests_configs/lru_cache.hpp"
#include "tests_structs.hpp"
#include "LRUcache.hpp"

using queries_list_t = std::vector<std::pair<bool, std::size_t>>;

class LRUcacheTest : public ::testing::TestWithParam<CacheTestConf> {
protected:
  webpage::page_t page;
};

TEST_P(LRUcacheTest, AllTests) {
  const auto& params = GetParam();
  cache_implementations::cache_t<webpage::page_t, std::size_t> cache(params.cache_size);

  for (auto [is_hit, index] : params.queries) {
    EXPECT_EQ(cache.lookup_update(page, index, webpage::slow_get_page), is_hit);
  }
}

// Define test cases
static std::vector<CacheTestConf> lru_test_cases = {
  lru_cache_tests::fromPresentation,
  lru_cache_tests::cacheSize1,
  lru_cache_tests::cap3_1,
  lru_cache_tests::cap3_2,
  lru_cache_tests::all_fit_big_cap
};

INSTANTIATE_TEST_SUITE_P(
  LRUcacheTests,
  LRUcacheTest,
  ::testing::ValuesIn(lru_test_cases),
  [](const ::testing::TestParamInfo<CacheTestConf>& info) {
    return info.param.test_name;
  }
);

#include <gtest/gtest.h>

#include "tests_configs/2q_cache.hpp"
#include "tests_structs.hpp"
#include "2qCache.hpp"

using queries_list_t = std::vector<std::pair<bool, std::size_t>>;

class Cache2QTest : public ::testing::TestWithParam<CacheTestConf> {
protected:
  page_t page;
};

TEST_P(Cache2QTest, AllTests) {
  const auto& params = GetParam();
  lru2q_cache_t<page_t, std::size_t> cache(params.cache_size);

  for (auto [is_hit, index] : params.queries) {
    EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);
  }
}

// Define test cases
static std::vector<CacheTestConf> lru2q_test_cases = {
  lru2q_cache_tests::fromPresentation,
  lru2q_cache_tests::cacheSize1,
  lru2q_cache_tests::cap3_1,
  lru2q_cache_tests::cap4_1,
  lru2q_cache_tests::cap5_1,
  lru2q_cache_tests::cap6_1,
  lru2q_cache_tests::all_fit_big_cap
};

INSTANTIATE_TEST_SUITE_P(
  LRU2QCacheTests,
  Cache2QTest,
  ::testing::ValuesIn(lru2q_test_cases),
  [](const ::testing::TestParamInfo<CacheTestConf>& info) {
    return info.param.test_name;
  }
);

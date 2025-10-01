#include <gtest/gtest.h>

#include "tests_configs/proph_cache.hpp"
#include "tests_structs.hpp"
#include "prophecyCache.hpp"

static std::vector<std::size_t> GetRequestsFromQueries(test_queries_t& queries) {
  std::vector<std::size_t> reqs = {};
  reqs.reserve(queries.size());
  for (auto& [_, ind] : queries) {
    reqs.emplace_back(ind);
  }

  return reqs;
}

class ProphecyCacheTest : public ::testing::TestWithParam<CacheTestConf> {
protected:
  page_t page;
};

TEST_P(ProphecyCacheTest, AllTests) {
  const auto& params = GetParam();
  test_queries_t queries = params.queries;
  prophecy_cache_t<page_t, std::size_t> cache(
    params.cache_size,
    GetRequestsFromQueries(queries)
  );
  for (auto [is_hit, index] : queries) {
    EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);
  }
}

// Define test cases
static std::vector<CacheTestConf> prophecy_cache_test_cases = {
  prophecy_cache_tests::fromPresentation,
  prophecy_cache_tests::cacheSize1,
  prophecy_cache_tests::period3_cap3,
  prophecy_cache_tests::period3_cap2,
  prophecy_cache_tests::reverse
};

INSTANTIATE_TEST_SUITE_P(
  ProphecyCacheTests,
  ProphecyCacheTest,
  ::testing::ValuesIn(prophecy_cache_test_cases),
  [](const ::testing::TestParamInfo<CacheTestConf>& info) {
    return info.param.test_name;
  }
);


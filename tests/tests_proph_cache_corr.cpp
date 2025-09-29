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

/*

Please don't beat me for using code gen, I've wanted each test to have it's own name
for easier debugging after test fail and parametrized test doesn't allow this,
even though it would be handy to simply write single TEST_P and iterate through
all test configurations

*/
#define ADD_PROPH_CACHE_TEST_CASE(testName)                                  \
  TEST(CacheTest, testName##ProphCase) {                                     \
    page_t page;                                                             \
    test_queries_t queries = prophecy_cache_tests::testName.queries;         \
    prophecy_cache_t<page_t, std::size_t> cache(                             \
      prophecy_cache_tests::testName.cache_size,                             \
      GetRequestsFromQueries(queries)                                        \
    );                                                                       \
    for (auto [is_hit, index] : queries) {                                   \
      EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);    \
    }                                                                        \
  }

ADD_PROPH_CACHE_TEST_CASE(fromPresentation)
ADD_PROPH_CACHE_TEST_CASE(cacheSize1)
ADD_PROPH_CACHE_TEST_CASE(period3_cap3)
ADD_PROPH_CACHE_TEST_CASE(period3_cap2)
ADD_PROPH_CACHE_TEST_CASE(reverse)

#undef ADD_PROPH_CACHE_TEST_CASE

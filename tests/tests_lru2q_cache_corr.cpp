#include <gtest/gtest.h>

#include "tests_configs/2q_cache.hpp"
#include "tests_structs.hpp"
#include "2qCache.hpp"

using queries_list_t = std::vector<std::pair<bool, size_t>>;

#define ADD_2Q_CACHE_TEST_CASE(testName)                                          \
  TEST(CacheTest, testName##Test2Qcache) {                                        \
    page_t page;                                                                  \
    lru2q_cache_t<page_t, size_t> cache(lru2q_cache_tests::testName.cache_size);  \
    for (auto [is_hit, index] : lru2q_cache_tests::testName.queries) {            \
      EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);         \
    }                                                                             \
  }

ADD_2Q_CACHE_TEST_CASE(fromPresentation)
ADD_2Q_CACHE_TEST_CASE(cacheSize1)
ADD_2Q_CACHE_TEST_CASE(cap3_1)
ADD_2Q_CACHE_TEST_CASE(cap4_1)
ADD_2Q_CACHE_TEST_CASE(cap5_1)
ADD_2Q_CACHE_TEST_CASE(cap6_1)
ADD_2Q_CACHE_TEST_CASE(all_fit_big_cap)

#undef ADD_2Q_CACHE_TEST_CASE

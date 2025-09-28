#include <gtest/gtest.h>

#include "tests_configs/lru_cache.hpp"
#include "tests_structs.hpp"
#include "LRUcache.hpp"

#define ADD_LRU_CACHE_TEST_CASE(testName)                                   \
  TEST(CacheTest, testName##TestFromPresentationLRUcache) {                 \
    page_t page;                                                            \
    cache_t<page_t, size_t> cache(lru_cache_tests::testName.cache_size);    \
    for (auto [is_hit, index] : lru_cache_tests::testName.queries) {        \
      EXPECT_EQ(cache.lookup_update(page, index, slow_get_page), is_hit);   \
    }                                                                       \
  }

ADD_LRU_CACHE_TEST_CASE(fromPresentation)
ADD_LRU_CACHE_TEST_CASE(cacheSize1)
ADD_LRU_CACHE_TEST_CASE(cap3_1)
ADD_LRU_CACHE_TEST_CASE(cap3_2)
ADD_LRU_CACHE_TEST_CASE(all_fit_big_cap)

#undef ADD_LRU_CACHE_TEST_CASE

// #include <gtest/gtest.h>

// #include "../LRUcache/include/LRUcacheStruct.hpp"

// struct CacheTest {
//   size_t cache_size;
//   std::vector<std::pair<bool, size_t>> queries;
// };

// template<size_t CacheSize>
// class LRUcacheTest : public testing::TestWithParam<std::pair<bool, size_t>> {
//  protected:
//   LRUcacheTest() : cache(CacheSize) {}
//   // void SetUp() override {
//   //   // GetParam().cache_size;
//   //   // cache(GetParam().cache_size);
//   // }
//   cache_t<page_t, size_t> cache;
// };

// using LRUcacheTestSize1 = LRUcacheTest<1>;
// using LRUcacheTestSize4 = LRUcacheTest<4>;

// static const CacheTest test1 = {
//   .cache_size = 1,
//   .queries = {
//     {false, 1},
//     {false, 2},
//     {false, 3},
//     {false, 1},
//     {false, 2},
//     {false, 3},
//     {true,  3},
//     {true,  3},
//     {false, 1}
//   }
// };

// static const CacheTest test2_from_presentation = {
//   .cache_size = 4,
//   .queries = {
//     {false, 1},
//     {false, 2},
//     {false, 3},
//     {false, 4},
//     {true,  1},
//     {true,  2},
//     {false, 5},
//     {true,  1},
//     {true,  2},
//     {true,  4},
//     {false, 3},
//     {true,  4}
//   }
// };

// // static const std::vector<CacheTest> params1 = {
// //   test1,
// //   test2_from_presentation
// // };

// TEST_P(LRUcacheTestSize1, CacheTestSize1) {
//   page_t page; // we are not using it here
//   auto [is_hit, number] = GetParam();
//   LOG_DEBUG_VARS(is_hit, number);
//   setLoggingLevel(DEBUG);
//   cache.dump_cache();
//   EXPECT_EQ(cache.lookup_update(page, number, slow_get_page), is_hit);
// }

// INSTANTIATE_TEST_SUITE_P(
//   CacheTestSize1,
//   LRUcacheTestSize1,
//   testing::Values(
//     std::make_pair(false, 1),
//     std::make_pair(false, 2),
//     std::make_pair(false, 3),
//     std::make_pair(false, 1),
//     std::make_pair(false, 2),
//     std::make_pair(false, 3),
//     std::make_pair(true,  3),
//     std::make_pair(true,  3),
//     std::make_pair(false, 1)
//   )
// );

// // INSTANTIATE_TEST_SUITE_P(
// //   CacheHandWrittenQueries,
// //   LRUcacheTestSize4,
// //   testing::ValuesIn(test2_from_presentation.queries)
// // );

// // TEST_F(LRUcacheTest, FromPresentation) {
// //   page_t page; // we are not using it here
// //   {
// //     cache_t<page_t, size_t> cache(1);
// //     EXPECT_FALSE(cache.lookup_update(page, 1,  slow_get_page));
// //     EXPECT_TRUE (cache.lookup_update(page, 2,  slow_get_page));
// //     EXPECT_TRUE (cache.lookup_update(page, 3,  slow_get_page));
// //     EXPECT_TRUE (cache.lookup_update(page, 4,  slow_get_page));
// //     EXPECT_FALSE(cache.lookup_update(page, 2,  slow_get_page));
// //     EXPECT_FALSE(cache.lookup_update(page, 1,  slow_get_page));
// //     EXPECT_FALSE(cache.lookup_update(page, 2,  slow_get_page));
// //     EXPECT_FALSE(cache.lookup_update(page, 1,  slow_get_page));
// //     EXPECT_FALSE(cache.lookup_update(page, 10, slow_get_page));
// //     EXPECT_TRUE (cache.lookup_update(page, 10, slow_get_page));
// //   }
// // }

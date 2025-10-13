#include <cassert>
#include <iostream>

#include "webPageStruct.hpp"
#include "LRUcache.hpp"
#include "prophecyCache.hpp"
#include "2qCache.hpp"

namespace err_msgs {
  inline const char* const EMPTY_QUERIES_LIST = "No queries were provided, num_of_queries == 0";
};

int main() {
  std::size_t max_cache_size = 0;
  std::size_t num_of_queries = 0;
  std::cin >> max_cache_size >> num_of_queries;

  if (num_of_queries == 0) {
    std::cerr << err_msgs::EMPTY_QUERIES_LIST << std::endl;
    exit(0);
  }

  std::vector<std::size_t> requests(num_of_queries);
  for (auto& it : requests) {
    std::cin >> it;
  }

#ifdef RUN_LRU_CACHE_
  cache_implementations::cache_t<webpage::page_t, std::size_t> cache(max_cache_size);
#elif RUN_2Q_CACHE_
  cache_implementations::lru2q_cache_t<webpage::page_t, std::size_t> cache(max_cache_size);
#elif RUN_PROPHECY_CACHE_
  cache_implementations::prophecy_cache_t<webpage::page_t, std::size_t> cache(max_cache_size, requests);
#else
  LOG_ERROR("Error: invalid debug option for target, no cache type was chosen...\n");
  exit(0);
#endif

  std::size_t hits = 0;
  for (std::size_t _ = 0; _ < num_of_queries; ++_) {
    webpage::page_t page;
    page.index = requests[_];
    if (cache.lookup_update(page, page.index, webpage::slow_get_page)) {
      hits += 1;
    }
  }

  std::cout << hits << std::endl;

  return 0;
}

/*

test from presentation:
4
12
1
2
3
4
1
2
5
1
2
4
3
4

*/

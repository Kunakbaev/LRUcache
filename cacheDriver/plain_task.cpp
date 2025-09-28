#include <iostream>

#include "webPageStruct.hpp"
#include "2qCache.hpp"

int main() {
  size_t max_cache_size = 0;
  size_t num_of_queries = 0;
  std::cin >> max_cache_size >> num_of_queries;
  lru2q_cache_t<page_t, size_t> cache(max_cache_size);

  size_t hits = 0;
  for (size_t _ = 0; _ < num_of_queries; ++_) {
    page_t page;
    std::cin >> page.index;
    if (cache.lookup_update(page, page.index, slow_get_page)) {
      hits += 1;
    }
  }

  std::cout << hits << std::endl;

  return 0;
}

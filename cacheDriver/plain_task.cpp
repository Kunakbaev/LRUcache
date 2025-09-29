#include <iostream>

#include "webPageStruct.hpp"
#include "2qCache.hpp"

int main() {
  std::size_t max_cache_size = 0;
  std::size_t num_of_queries = 0;
  std::cin >> max_cache_size >> num_of_queries;
  lru2q_cache_t<page_t, std::size_t> cache(max_cache_size);

  std::size_t hits = 0;
  for (std::size_t _ = 0; _ < num_of_queries; ++_) {
    page_t page;
    std::cin >> page.index;
    if (cache.lookup_update(page, page.index, slow_get_page)) {
      hits += 1;
    }
  }

  std::cout << hits << std::endl;

  return 0;
}

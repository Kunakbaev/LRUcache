#include <vector>
#include <random>
#include <cmath>

#include "../LRUcache/include/LRUcacheStruct.hpp"
#include "../2qCache/2qCache.hpp"
#include "../prophecyCache/include/prophecyCache.hpp"

static const size_t TESTS           = 100;
static const size_t MAX_NUM_OF_REQS = 100;
static const size_t MAX_PAGE_INDEX2 = 50;
static const size_t MAX_CACHE_SZ    = 10;

static const char* const BETTER_THAN_BEST_ERR_MSG = "Error: lru2q cache is better than most optimal, \"future seeing\" cache";

static std::mt19937_64 rnd(228);

static int dice(int low, int high) {
  return std::uniform_int_distribution<int>(low, high)(rnd);
}

static std::vector<size_t> genRandomRequests(
  size_t num_of_requests,
  size_t max_page_ind
) {
  std::vector<size_t> requests(num_of_requests);
  for (auto& ind : requests) {
    ind = static_cast<size_t>(dice(1, (int)max_page_ind));
  }

  return requests;
}

template<class T>
static size_t getNumOfHits4cache(
  T& cache,
  const std::vector<size_t>& requests
) {
  size_t hits = 0;
  for (auto index : requests) {
    page_t page;
    page.index = index;
    if (cache.lookup_update(page, index, slow_get_page)) {
      hits += 1;
    }
  }

  return hits;
}

int main() {
  size_t lru2q_cache_wins = 0;
  std::vector<size_t> requests = {};
  for (size_t _ = 0; _ < TESTS; ++_) {
    size_t num_of_requests = static_cast<size_t>(dice(1, MAX_NUM_OF_REQS));
    size_t max_page_ind    = static_cast<size_t>(dice(1, MAX_PAGE_INDEX2));
    size_t max_cache_sz    = static_cast<size_t>(dice(1, MAX_CACHE_SZ));
    requests = genRandomRequests(num_of_requests, max_page_ind);

    cache_t<page_t, size_t>            lru_cache(max_cache_sz);
    lru2q_cache_t<page_t, size_t>    lru2q_cache(max_cache_sz);
    prophecy_cache_t<page_t, size_t> proph_cache(max_cache_sz, requests);

    size_t   lru_hits = getNumOfHits4cache(lru_cache,   requests);
    size_t lru2q_hits = getNumOfHits4cache(lru2q_cache, requests);
    size_t proph_hits = getNumOfHits4cache(proph_cache, requests);

    lru2q_cache_wins += lru2q_hits > lru_hits;

    if (lru2q_hits > proph_hits) { // error: smth went wrong
      std::cout << BETTER_THAN_BEST_ERR_MSG << std::endl;
      std::cout << "num of requests:" << std::endl;
      for (auto& it : requests) {
        std::cout << it << " ";
      }
      std::cout << std::endl;

      std::cout << "lru      hits : " << lru_hits   << std::endl;
      std::cout << "lru2q    hits : " << lru2q_hits << std::endl;
      std::cout << "prophecy hits : " << proph_hits << std::endl;

      break;
    }
  }

  double ratio = static_cast<double>(lru2q_cache_wins) / static_cast<double>(TESTS);
  int percent = static_cast<int>(std::ceil(ratio * 100));
  std::cout << "LRU2q cache is better than original LRU cache in " << percent << "% of cases" << std::endl;

  return 0;
}

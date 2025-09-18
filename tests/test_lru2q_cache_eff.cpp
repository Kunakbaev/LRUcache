#include <vector>
#include <random>
#include <cmath>

#include "../LRUcache/include/LRUcacheStruct.hpp"
#include "../2qCache/2qCache.hpp"
#include "../prophecyCache/include/prophecyCache.hpp"

static const size_t TESTS           = 100;
static const size_t MAX_NUM_OF_REQS = 100;
static const size_t MAX_PAGE_INDEX2 = 5;
static const size_t MAX_CACHE_SZ    = 20;
static const double ZIPF_ALPHA      = 0.8;

static const char* const BETTER_THAN_BEST_ERR_MSG = "Error: lru2q cache is better than most optimal, \"future seeing\" cache";

std::mt19937_64 rnd(228);

int GetIntDice(int low, int high) {
  return std::uniform_int_distribution<int>(low, high)(rnd);
}

double GetDoubleDice(double low, double high) {
  return std::uniform_real_distribution<double>(low, high)(rnd);
}

void PrecountZipfProbs(
  std::vector<double>& pref_zipf_probs,
  std::size_t          num_of_pages
) {
  pref_zipf_probs.resize(num_of_pages);
  for (size_t i = 1; i <= num_of_pages; ++i) {
    pref_zipf_probs[i - 1] =
      std::pow(static_cast<double>(i) / static_cast<double>(num_of_pages), ZIPF_ALPHA);
  }
}

std::vector<size_t> GenRandomRequests(
  size_t num_of_requests,
  size_t max_page_ind
) {
  std::vector<double> pref_zipf_probs;
  PrecountZipfProbs(pref_zipf_probs, max_page_ind);

  std::vector<size_t> requests(num_of_requests);
  for (auto& ind : requests) {
    double tmp = GetDoubleDice(0.0, 1.0);
    ind = std::lower_bound(pref_zipf_probs.begin(), pref_zipf_probs.end(), tmp) - pref_zipf_probs.begin();
    ++ind;
    LOG_DEBUG_VARS(tmp, ind, max_page_ind);
    // ind = static_cast<size_t>(dice(1, (int)max_page_ind));
  }

  return requests;
}

template<class T>
static size_t GetNumOfHits4cache(
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
  setLoggingLevel(DEBUG);
  LOG_DEBUG("Hello");

  size_t lru2q_cache_wins = 0;
  std::vector<size_t> requests = {};
  for (size_t _ = 0; _ < TESTS; ++_) {
    size_t num_of_requests = static_cast<size_t>(GetIntDice(1, MAX_NUM_OF_REQS));
    size_t max_page_ind    = static_cast<size_t>(GetIntDice(1, MAX_PAGE_INDEX2));
    size_t max_cache_sz    = static_cast<size_t>(GetIntDice(3, MAX_CACHE_SZ));
    requests = GenRandomRequests(num_of_requests, max_page_ind);

    cache_t<page_t, size_t>            lru_cache(max_cache_sz);
    lru2q_cache_t<page_t, size_t>    lru2q_cache(max_cache_sz);
    prophecy_cache_t<page_t, size_t> proph_cache(max_cache_sz, requests);

    size_t   lru_hits = GetNumOfHits4cache(lru_cache,   requests);
    size_t lru2q_hits = GetNumOfHits4cache(lru2q_cache, requests);
    size_t proph_hits = GetNumOfHits4cache(proph_cache, requests);

    lru2q_cache_wins += lru2q_hits > lru_hits;
    if (lru2q_hits > proph_hits || 1) { // error: smth went wrong
      std::cout << BETTER_THAN_BEST_ERR_MSG << std::endl;
      std::cout << "max cache size :" << max_cache_sz << std::endl;
      std::cout << "num of requests:" << num_of_requests << std::endl;
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

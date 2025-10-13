#include <vector>
#include <random>
#include <cmath>
#include <iomanip>

#include "LRUcache.hpp"
#include "2qCache.hpp"
#include "prophecyCache.hpp"

namespace tests_specifications {

inline constexpr size_t TOTAL_NUM_TESTS = 100;
inline constexpr size_t MAX_NUM_OF_REQS = 100;
inline constexpr size_t MAX_PAGE_INDEX2 = 1000;
inline constexpr size_t MAX_CACHE_SZ    = 200;
inline constexpr double ZIPF_ALPHA      = 0.8;

// NOTE: this is not that crucial, but without this condition
// tests won't show anything interesting
// hit rate for all caches will be 100%
static_assert(MAX_CACHE_SZ < MAX_PAGE_INDEX2);
static_assert(ZIPF_ALPHA + 1e-6 < 1.0);

};

namespace err_msgs {
  inline const char* const BETTER_THAN_BEST_ERR_MSG
    = "Error: lru2q cache is better than most optimal, \"future seeing\" cache";
};

namespace my_random {
  inline std::mt19937_64 rnd(228);

  inline int GetIntDice(int low, int high) {
    return std::uniform_int_distribution<int>(low, high)(rnd);
  }

  inline double GetDoubleDice(double low, double high) {
    return std::uniform_real_distribution<double>(low, high)(rnd);
  }
};

struct one_test_result_t {
  std::size_t lru_hits;
  std::size_t lru2q_hits;
  std::size_t prophecy_hits;

  one_test_result_t(std::size_t lru, std::size_t lru2q, std::size_t prophecy)
    : lru_hits(lru), lru2q_hits(lru2q), prophecy_hits(prophecy) {}
};

namespace {

void PrecountZipfProbs(
  std::vector<double>& pref_zipf_probs,
  std::size_t          num_of_pages
) {
  pref_zipf_probs.resize(num_of_pages);
  for (std::size_t i = 1; i <= num_of_pages; ++i) {
    pref_zipf_probs[i - 1] =
      std::pow(static_cast<double>(i) / static_cast<double>(num_of_pages),
               tests_specifications::ZIPF_ALPHA);
  }
}

std::vector<std::size_t> GenRandomRequests(
  std::size_t num_of_requests,
  std::size_t max_page_ind
) {
  std::vector<double> pref_zipf_probs;
  PrecountZipfProbs(pref_zipf_probs, max_page_ind);

  std::vector<std::size_t> requests(num_of_requests);
  for (auto& ind : requests) {
    double tmp = my_random::GetDoubleDice(0.0, 1.0);
    ind = std::lower_bound(pref_zipf_probs.begin(),
                           pref_zipf_probs.end(), tmp) - pref_zipf_probs.begin() + 1;
  }

  return requests;
}

template<class T>
std::size_t GetNumOfHits4cache(
  T& cache,
  const std::vector<std::size_t>& requests
) {
  std::size_t hits = 0;
  for (auto index : requests) {
    webpage::page_t page;
    page.index = index;
    if (cache.lookup_update(page, index, webpage::slow_get_page)) {
      hits += 1;
    }
  }

  return hits;
}

void show_hit_perf_table(const std::vector<one_test_result_t>& test_results) {
  std::cout << "Caches hit performance on tests:\n";
  std::cout << std::left 
            << std::setw(10) << "test id" << "|"
            << std::setw(10) << "lru hits" << "|" 
            << std::setw(10) << "2q hits" << "|"
            << std::setw(10) << "lru delta" << "|"
            << std::setw(10) << "2q delta" << "|"
            << std::setw(15) << "prophecy hits" << "\n";

  std::size_t test_ind = 1;
  for (const auto& [lru_hits, lru2q_hits, prophecy_hits] : test_results) {
      std::cout << std::left
                << std::setw(10) << test_ind << "|"
                << std::setw(10) << lru_hits << "|"
                << std::setw(10) << lru2q_hits << "|"
                << std::setw(10) << static_cast<int>(lru_hits - prophecy_hits) << "|"
                << std::setw(10) << static_cast<int>(lru2q_hits - prophecy_hits) << "|"
                << std::setw(15) << prophecy_hits << "\n";
      ++test_ind;
  }
}

}

int main() {
  setLoggingLevel(DEBUG);

  std::size_t lru2q_cache_wins = 0;
  std::vector<size_t> requests = {};
  std::vector<one_test_result_t> test_results = {};
  for (std::size_t _ = 0; _ < tests_specifications::TOTAL_NUM_TESTS; ++_) {
    std::size_t num_of_requests = static_cast<std::size_t>(my_random::GetIntDice(1, tests_specifications::MAX_NUM_OF_REQS));
    std::size_t max_page_ind    = static_cast<std::size_t>(my_random::GetIntDice(1, tests_specifications::MAX_PAGE_INDEX2));
    std::size_t max_cache_sz    = static_cast<std::size_t>(my_random::GetIntDice(3, tests_specifications::MAX_CACHE_SZ));
    requests = GenRandomRequests(num_of_requests, max_page_ind);

    cache_implementations::         cache_t<webpage::page_t, std::size_t> lru_cache(max_cache_sz);
    cache_implementations::   lru2q_cache_t<webpage::page_t, std::size_t> lru2q_cache(max_cache_sz);
    cache_implementations::prophecy_cache_t<webpage::page_t, std::size_t> proph_cache(max_cache_sz, requests);

    std::size_t   lru_hits = GetNumOfHits4cache(lru_cache,   requests);
    std::size_t lru2q_hits = GetNumOfHits4cache(lru2q_cache, requests);
    std::size_t proph_hits = GetNumOfHits4cache(proph_cache, requests);

    test_results.emplace_back(lru_hits, lru2q_hits, proph_hits);

    lru2q_cache_wins += lru2q_hits >= lru_hits;
    if (lru2q_hits > proph_hits) { // error: smth went wrong
      std::cout << err_msgs::BETTER_THAN_BEST_ERR_MSG << "\n";
      std::cout << "max cache size :" << max_cache_sz << "\n";
      std::cout << "num of requests:" << num_of_requests << "\n";
      for (auto& it : requests) {
        std::cout << it << " ";
      }
      std::cout << "\n";

      std::cout << "lru      hits : " << lru_hits   << "\n";
      std::cout << "lru2q    hits : " << lru2q_hits << "\n";
      std::cout << "prophecy hits : " << proph_hits << std::endl;

      break;
    }
  }

  double ratio = static_cast<double>(lru2q_cache_wins) /
                 static_cast<double>(tests_specifications::TOTAL_NUM_TESTS);
  int percent = static_cast<int>(std::ceil(ratio * 100));
  std::cout << "LRU2q cache is better than original LRU cache in " << percent << "% of cases" << std::endl;

  show_hit_perf_table(test_results);

  return 0;
}

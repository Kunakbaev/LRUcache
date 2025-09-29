#include <cassert>
#include <iostream>

#include "logLib.hpp"
#include "webPageStruct.hpp"
#include "LRUcache.hpp"
#include "prophecyCache.hpp"
#include "2qCache.hpp"

namespace err_msgs {
  inline const char* const STR_NOT_NUM_ERR_MSG        = "Error: input string must be a positive integer...";
  inline const char* const NUM_IS_TOO_BIG_ERR_MSG     = "Error: input number is too big...";
  inline const char* const INVALID_NUM_FORMAT_ERR_MSG = "Invalid input format, it's either not a number or it's bigger than ";
};

#ifndef TEST_PERF_
namespace input_specs {
  inline const std::size_t MAX_CACHE_SIZE                  = 30;
  inline const std::size_t MAX_NUM_OF_QUERIES              = 1024;
};
 
static bool try2readPositiveInt(
  size_t&      num,
  const std::size_t max_value
) {
  std::string input = "";
  std::cin >> input;

  num = 0;
  const static std::size_t NUM_BASE = 10;
  for (auto digit : input) {
    if (!isdigit(digit)) {
      LOG_ERROR(err_msgs::STR_NOT_NUM_ERR_MSG);
      return false;
    }

    // TODO: overflow check
    num *= NUM_BASE;
    num += static_cast<std::size_t>(digit - '0');
    if (num > max_value) {
      LOG_ERROR(err_msgs::NUM_IS_TOO_BIG_ERR_MSG);
      return false;
    }
  }

  return true;
}

static std::size_t SafelyReadPositiveInt(
  const std::string& inpPrompt,
  const std::size_t       max_value
) {
  std::size_t num = 0;
  std::cout << inpPrompt;
  while (!try2readPositiveInt(num, max_value)) {
    std::cout << err_msgs::INVALID_NUM_FORMAT_ERR_MSG
      << max_value << std::endl;
    std::cout << inpPrompt;
  };

  return num;
}

static void Input(
  std::size_t& max_cache_size,
  std::size_t& num_of_queries)
{
  max_cache_size = SafelyReadPositiveInt("Max cache size: ", input_specs::MAX_CACHE_SIZE);
  num_of_queries = SafelyReadPositiveInt("Num of queries: ", input_specs::MAX_NUM_OF_QUERIES);
}
#endif

int main() {
  setLoggingLevel(DEBUG);
  LOG_DEBUG("Hello world!");

  std::size_t max_cache_size = 0;
  std::size_t num_of_queries = 0;
#ifndef TEST_PERF_
  Input(max_cache_size, num_of_queries);
#else
  std::cin >> max_cache_size >> num_of_queries;
#endif
  LOG_DEBUG_VARS(max_cache_size, num_of_queries);

  std::vector<std::size_t> requests(num_of_queries);
  for (auto& it : requests) {
#ifndef TEST_PERF_
    it = SafelyReadPositiveInt("Input page index: ", MAX_PAGE_INDEX);
#else
    std::cin >> it;
#endif
  }

  // MEGA CRINGE:
  bool anyCacheCreated = false;
#ifdef RUN_LRU_CACHE_
  anyCacheCreated = true;
  cache_t<page_t, std::size_t> cache(max_cache_size);
#endif
#ifdef RUN_2Q_CACHE_
  anyCacheCreated = true;
  lru2q_cache_t<page_t, std::size_t> cache(max_cache_size);
#endif
#ifdef RUN_PROPHECY_CACHE_
  anyCacheCreated = true;
  prophecy_cache_t<page_t, std::size_t> cache(max_cache_size, requests);
#endif
  if (!anyCacheCreated) {
    LOG_ERROR("Error: invalid debug option for target, no cache type was chosen...\n");
    exit(0);
  }

  std::size_t hits = 0;
  for (std::size_t _ = 0; _ < num_of_queries; ++_) {
    page_t page;
    page.index = requests[_];
    LOG_DEBUG("-----------------");
    LOG_DEBUG_VARS(page.index);
    if (cache.lookup_update(page, page.index, slow_get_page)) {
      hits += 1;
    }

#ifdef _DEBUG
    cache.dump_cache();
#endif
  }

#ifndef TEST_PERF_
  double percent = static_cast<double>(hits) / static_cast<double>(num_of_queries);     
  std::cout << "Cache hits : " << hits << " percentage of hits : "
    << percent << std::endl;
#else
  std::cout << hits << std::endl;
#endif

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

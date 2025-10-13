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
  inline const char* const EMPTY_QUERIES_LIST         = "No queries were provided, num_of_queries == 0";
};

namespace input_specs {
  inline const std::size_t MAX_CACHE_SIZE             = 100;
  inline const std::size_t MAX_NUM_OF_QUERIES         = 1024;
};
 
static bool Try2readPositiveInt(
  size_t&           num,
  const std::size_t max_value
) {
  std::string input;
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

  return num > 0;
}

static std::size_t SafelyReadPositiveInt(
  const std::string& inp_prompt,
  std::size_t        max_value
) {
  std::size_t num = 0;
  std::cout << inp_prompt;
  while (!Try2readPositiveInt(num, max_value)) {
    std::cout << err_msgs::INVALID_NUM_FORMAT_ERR_MSG
      << max_value << std::endl;
    std::cout << inp_prompt;
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

int main() {
  setLoggingLevel(DEBUG);

  std::size_t max_cache_size = 0;
  std::size_t num_of_queries = 0;
  Input(max_cache_size, num_of_queries);
  LOG_DEBUG_VARS(max_cache_size, num_of_queries);

  if (num_of_queries == 0) {
    std::cerr << err_msgs::EMPTY_QUERIES_LIST << std::endl;
    exit(0);
  }

  std::vector<std::size_t> requests(num_of_queries);
  for (auto& it : requests) {
    it = SafelyReadPositiveInt("Input page index: ",
                               std::numeric_limits<std::size_t>::max());
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
    LOG_DEBUG_VARS(page.index);
    if (cache.lookup_update(page, page.index, webpage::slow_get_page)) {
      hits += 1;
      LOG_WARNING("-----------hit----------");
    }

#ifdef _DEBUG
    cache.dump_cache();
#endif
  }

  double percent = static_cast<double>(hits) / static_cast<double>(num_of_queries);     
  std::cout << "Cache hits : " << hits << " percentage of hits : "
    << percent << std::endl;

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

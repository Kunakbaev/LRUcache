#include <cassert>
#include <iostream>

#include "../3dPartyModules/LoggerLib/include/logLib.hpp"
#include "../webPage/include/webPageStruct.hpp"
#include "../LRUcache/include/LRUcacheStruct.hpp"

static const char* const STR_NOT_NUM_ERR_MSG        = "Error: input string must be a positive integer...";
static const char* const NUM_IS_TOO_BIG_ERR_MSG     = "Error: input number is too big...";
static const char* const INVALID_NUM_FORMAT_ERR_MSG = "Invalid input format, it's either not a number or it's bigger than ";
static const size_t MAX_CACHE_SIZE                  = 8;
static const size_t MAX_NUM_OF_QUERIES              = 20;

// returns 
static bool try2readPositiveInt(
  size_t&      num,
  const size_t max_value
) {
  std::string input = "";
  std::cin >> input;

  num = 0;
  const static size_t NUM_BASE = 10;
  for (auto digit : input) {
    if (!isdigit(digit)) {
      LOG_ERROR(STR_NOT_NUM_ERR_MSG);
      return false;
      // assert(false && STR_NOT_NUM_ERR_MSG);
    }

    // TODO: overflow check
    num *= NUM_BASE;
    num += static_cast<size_t>(digit - '0');
    if (num > max_value) {
      LOG_ERROR(NUM_IS_TOO_BIG_ERR_MSG);
      return false;
      // assert(false && NUM_IS_TOO_BIG_ERR_MSG);
    }
  }

  return true;
}

static size_t SafelyReadPositiveInt(
  const std::string& inpPrompt,
  const size_t       max_value
) {
  size_t num = 0;
  std::cout << inpPrompt;
  while (!try2readPositiveInt(num, max_value)) {
    std::cout << INVALID_NUM_FORMAT_ERR_MSG
      << max_value << std::endl;
    std::cout << inpPrompt;
  };

  return num;
}

static void Input(
  size_t& max_cache_size,
  size_t& num_of_queries)
{
  max_cache_size = SafelyReadPositiveInt("Max cache size: ", MAX_CACHE_SIZE);
  num_of_queries = SafelyReadPositiveInt("Num of queries: ", MAX_NUM_OF_QUERIES);
}

int main() {
  setLoggingLevel(DEBUG);

  size_t max_cache_size = 0;
  size_t num_of_queries = 0;
  Input(max_cache_size, num_of_queries);
  LOG_DEBUG_VARS(max_cache_size, num_of_queries);

  cache_t<page_t, size_t> cache(max_cache_size);

  size_t hits = 0;
  for (size_t _ = 0; _ < num_of_queries; ++_) {
    page_t page;
    page.index = SafelyReadPositiveInt("Input page index: ", MAX_PAGE_INDEX);
    // size_t page_index;
    // std::cin >> page_index;
    // page.set_page_index(page_index);

    assert(std::cin.good());
    LOG_DEBUG_VARS(page.index);
    if (cache.lookup_update(page, page.index, slow_get_page)) {
      hits += 1;
    }

    cache.dump_cache();
  }

  double percent = static_cast<double>(hits) / static_cast<double>(num_of_queries);     
  std::cout << "Cache hits : " << hits << " percentage of hits : "
    << percent << std::endl;

  return 0;
}
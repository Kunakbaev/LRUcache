#pragma once

#include <map>
#include <set>
#include <stack>
#include <vector>
// #include <list>
// #include <unordered_map>

#include "../../3dPartyModules/LoggerLib/include/logLib.hpp"
#include "../../webPage/include/webPageStruct.hpp"

template <typename T>
struct set_comparator_t
{
  bool operator()(
    const std::pair<size_t, T>& one,
    const std::pair<size_t, T>& two
  ) const {
    // LOG_DEBUG_VARS(one.first, two.first, "compare");
    return one.first < two.first;
  }
};

template <typename T, typename KeyT = int>
class prophecy_cache_t {
 private:
  const size_t max_cache_sz_ = 0;

  // std::list<T> cache_ = {};
  // using ListIt = typename std::list<T>::iterator;
  // std::unordered_map<KeyT, ListIt> hash_ = {};

  // for each number from requests array we store list of its positions
  std::map<KeyT, std::stack<size_t>> positions = {};
  // .first  = position of index in requests arrays
  // .second = page structure
  std::set<std::pair<size_t, T>, set_comparator_t<T>> cache_ = {};
  //const std::vector<KeyT>& requests_;

 public:
  prophecy_cache_t(size_t max_cache_sz, const std::vector<KeyT>& requests)
      : max_cache_sz_(max_cache_sz) {
    for (int i = (int)requests.size(); i >= 0; --i) {
      KeyT ind = requests[(size_t)i];
      positions[ind].push((size_t)i);
    }

    // for (auto& [ind, arr] : positions) {
    //   //cache.insert(std::make_pair(arr.top(), ind));
    //   //arr.push(arr.top());
    // }
  }

  bool is_full() const {
    return cache_.size() == max_cache_sz_;
  }

  // returns true on cache hit and false on cache miss
  template <typename F>
  bool lookup_update(T& element, KeyT ind, F slow_get_page) {
    assert(!positions[ind].empty());
    size_t cur_pos = positions[ind].top();
    positions[ind].pop();
    size_t nxt_pos = (size_t)-1; // aka INF
    if (!positions[ind].empty()) {
      nxt_pos = positions[ind].top();
    }

    page_t _;
    // LOG_DEBUG_VARS(ind, cur_pos);
    auto hit = cache_.find(std::make_pair(cur_pos, _));
    if (hit != cache_.end()) { // index found, it's a cache hit
      element = hit->second;
      // LOG_DEBUG_VARS(hit->first);
      // LOG_WARNING("HIT");

      // we need to update set elem, as ind of nxt occur has changed
      cache_.erase(hit);
      cache_.insert(std::make_pair(nxt_pos, element));
      return true;
    }
    // LOG_WARNING("MISS");

    // cache miss
    slow_get_page(ind, element);
    bool wasFull = is_full();
    // TODO: redundant page copy?
    // we need to update set elem, as ind of nxt occur has changed
    cache_.insert(std::make_pair(nxt_pos, element));
    if (!wasFull) return false;

    // even if cache was full, we still have to add new page to set
    // because maybe it's not used in the future, so there's no need
    // to actually add it
    cache_.erase(*cache_.rbegin()); // delete one with highest future use
    return false;
  }

#ifdef _DEBUG
  void dump_cache() const {
    LOG_DEBUG("Cache dump:");
    for (auto [pos, elem_it] : cache_) {
      LOG_DEBUG_VARS(pos, elem_it.index);
    }
  }
#endif
};

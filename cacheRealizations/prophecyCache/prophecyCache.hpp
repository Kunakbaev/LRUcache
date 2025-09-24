#pragma once

#include <map>
#include <set>
#include <stack>
#include <vector>

#include "logLib.hpp"
#include "webPageStruct.hpp"

template <typename T>
struct set_comparator_t
{
  bool operator()(
    const std::pair<size_t, T>& one,
    const std::pair<size_t, T>& two
  ) const {
    return one.first < two.first;
  }
};

template <typename T, typename KeyT = int>
class prophecy_cache_t {
 private:
  const size_t max_cache_sz_ = 0;

  // for each number from requests array we store list of its positions
  std::map<KeyT, std::stack<size_t>> positions = {};
  // .first  = position of index in requests arrays
  // .second = page structure
  std::set<std::pair<size_t, T>, set_comparator_t<T>> cache_ = {};

 public:
  prophecy_cache_t(size_t max_cache_sz, const std::vector<KeyT>& requests)
      : max_cache_sz_(max_cache_sz) {
    assert(max_cache_sz >= 1);
    for (int i = (int)requests.size(); i >= 0; --i) {
      KeyT ind = requests[(size_t)i];
      positions[ind].push((size_t)i);
    }
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
    auto hit = cache_.find(std::make_pair(cur_pos, _));
    if (hit != cache_.end()) { // index found, it's a cache hit
      element = hit->second;

      // we need to update set elem, as ind of nxt occur has changed
      cache_.erase(hit);
      cache_.insert(std::make_pair(nxt_pos, element));
      return true;
    }

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

// Is this really bad? On lecture there was same example but with class field
#ifdef _DEBUG
  void dump_cache() const {
    LOG_DEBUG("Cache dump:");
    for (auto [pos, elem_it] : cache_) {
      LOG_DEBUG_VARS(pos, elem_it.index);
    }
  }
#endif
};

#pragma once

#include <list>
#include <unordered_map>

#include "../../3dPartyModules/LoggerLib/include/logLib.hpp"
#include "../../webPage/include/webPageStruct.hpp"

template <typename T, typename KeyT = int>
class cache_t {
 private:
  const size_t max_cache_sz_ = 0;
  std::list<T> cache_ = {};

  using ListIt = typename std::list<T>::iterator;
  std::unordered_map<KeyT, ListIt> hash_ = {};

 public:
  cache_t(size_t max_cache_sz) : max_cache_sz_(max_cache_sz) {}

  bool is_full() const {
    return cache_.size() == max_cache_sz_;
  }

  // returns true on cache hit and false on cache miss
  template <typename F>
  bool lookup_update(T& element, KeyT key, F slow_get_page) {
    auto hit = hash_.find(key);
    if (hit == hash_.end()) { // not found
      LOG_DEBUG("page not found");
      if (is_full()) {
        LOG_DEBUG("cache is full...");
        hash_.erase(cache_.back().index);
        cache_.pop_back();
      }

      // fallback to slow method
      slow_get_page(key, element);
      cache_.push_front(element);
      hash_[key] = cache_.begin();
      return false;
    }

    LOG_DEBUG("cache hit...");
    auto elem_it = hit->second;
    element = *elem_it;
    if (elem_it != cache_.begin()) {
      cache_.splice(cache_.begin(), cache_,
        elem_it, std::next(elem_it));
    }

    return true;
  }

#ifdef _DEBUG
  void dump_cache() const {
    LOG_DEBUG("Cache dump:");
    for (const T& elem_it : cache_) {
      LOG_DEBUG_VARS(elem_it.index);
    }
  }
#endif
};
